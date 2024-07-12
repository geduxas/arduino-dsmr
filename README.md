# Dutch Smart meter (DSMR) parser

This is a library for parsing messages from Dutch smart meters, through
their P1 port.

This code was originally written for Arduino by 
[matthijskooijman](https://github.com/matthijskooijman) and has been adapted
for use outside of the Arduino framework.

## Differences in this fork

What is different in this fork?
Parsing is done on `char*` instead of `const char*`. This enables in-buffer
parsing. Parsed data is not copied into separate variables. Instead, parsed
data just points to the right location in the input data. For c-strings to
work, a null-terminator is inserted into the input data. Yes, your input
data buffer is altered and therefore is is not `const`.

Consequences:
- this fork might not fit in your code
- upside: no duplication of string-data
- except for the `fullError`-function, no dynamic memory is allocated. This
  function now has one big attention point: it returns a pointer which you
  have to free.

## Protocol

Every smart meter in the Netherlands has to comply with the Dutch Smart
Meter Requirements (DSMR). At the time of writing, DSMR 5.0.2 is the version
in use in Belgium. This code should support both the 4.x and 5.0 specifications.
3.x meters might also work, but this has not been verified or tested.

A typical P1 message looks like this:

```
/KFM5KAIFA-METER

1-0:1.8.1(000671.578*kWh)
1-0:1.7.0(00.318*kW)
!1E1D
```

This includes an identification header at the top, a checksum at the
bottom, and one or more lines of data in the middle. This example is
really stripped down, real messages will have more data in them.

The first part of the line (e.g. `1-0:1.8.1`) is the (OBIS) id of the
field, which defines the meaning and format of the rest of the line.

## Parsing a message

Unlike other solutions floating around (which typically do some pattern
matching to extract the data they need), this code properly parses
messages, verifying the checksum and really parses each line according
to the specifications. This should make for more reliable parsing, and
allows for useful parser error messages:

```
1-0:1.8.1(000671.578*XWh)
                      ^
Error: Invalid unit

1-0:1.8.1(0006#71.578*kWh)
              ^
Error: Invalid number

!6F4A
  ^
Checksum mismatch
```

This library uses C++ templates extensively. This allows defining a
custom datatype by listing the fields you are interested in, and then
all necessary parsing will happen automatically. The code generated
parses each line in the message in turn and for each line loops over the
fields in the datatype to find one whose ID matches. If found, the value
is parsed and stored into the corresponding field.

As an example, consider we want to parse the identification and current
power fields in the example message above. We define a datatype:

```cpp
using MyData = ParsedData<
  /* String */ identification,
  /* FixedValue */ power_delivered
>;
```

The above defines a struct with members for each field to be parsed.
For each field, there is also an associated `xxx_present` member, which
can be used to check whether the field was present in the parsed data
(if it is false, the associated field contains uninitialized data).
There is some extra stuff in the background, but the `MyData` can be
used just like the below struct. It also takes up the same amount of
space.

```
struct MyData {
  bool identification_present;
  String identification;
  bool power_delivered_present;
  FixedValue power_delivered;
};
```

After this, call the parser. By passing our custom datatype defined
above, the parser knows what fields to look for.

```cpp
MyData data;
ParseResult<void> res = P1Parser::parse(&data, msg, lengthof(msg));
```

Finally, we can check if the parsing was succesful and access the parsed
values as members of `data`: (Arduino framework code)

```cpp
if (!res.err && res.all_present()) {
  // Succesfully parsed, print results:
  Serial.println(data.identification);
  Serial.print(data.power_delivered.int_val());
  Serial.println("W");
}
```

In this case, we check whether parsing was successful, but also check
that all defined fields were present in the parsed message (using the
`all_present()` method), to prevent printing undefined values. If you
want to support optional fields, you can use the `xxx_present` members
for each field individually instead.

Additionally, this template approach allows looping over all available
fields in a generic way, for example to print the parse results with
just a few lines of code. See the parse and read examples for how this
works.

Note that these examples contain the full list of supported fields,
which causes parsing and printing code to be generated for all those
fields, even if they are not present in the output you want to parse. It
is recommended to limit the list of fields to just the ones that you
need, to make the parsing and printing code smaller and faster.

## Parsed value types

Some values are parsed to a `const char*` value (null-terminated) or
C++ integer type, those should be fairly straightforward. There are
three special types that need some explanation: `FixedValue` and
`TimestampedFixedValue`.

When looking at the DSMR P1 format, it defines a floating point format.
It is described as `Fn(x,y)`, where `n` is the total number of (decimal)
digits, of which at least `x` and at most `y` are behind the decimal
separator (e.g. fractional digits).

However, this floating point format is a lot more limited than the C
`float` format. For one, it is decimal-based, not binary. Furthermore,
the decimal separator doesn't float very far, the biggest value for `y`
used is 3. Even more, it seems that for any given field, there is no
actual floating involved, fields have `x` equal to `y`, so the number of
fractional digits is fixed.

Because of this, parsing into a `float` value isn't really useful (and
on the Arduino, which doesn't have an FPU, very inefficient too). For
this reason, we use the `FixedValue` type, which stores the value as an
integer, in thousands of the original unit. This means that a value of
1.234kWh is stored as 1234 (effectively the value has been translated to
Wh).

If you access the field directly, it will automatically be converted to
`float`, keeping the original value. Alternatively, if an integer
version is sufficient, you can call the `int_val()` method to get the
integer version returned.

```cpp
// Print as float, in kW
Serial.print(data.power_delivered);
// Print as integer, in W
Serial.print(data.power_delivered.int_val());
```

Additionally there is a `TimestampedFixedValue` method, which works
identically, but additionally has a `timestamp()` method which returns
the timestamp sent along with the value.

These timestamps are returned as a `const char*` (null-terminated),
exactly as present in the P1 message (YYMMDDhhmmssX, where X is S or
W for summer- or wintertime). Parsing these into something like a UNIX
timestamp is tricky (think leap years and seconds) and of limited use,
so this just keeps the original format.

## Connecting the P1 port

The P1 port essentially consists of three parts:

- A 5V power supply (this was not present in 3.x).
- A serial TX pin. The data pin of the P1-port is open drain (it is
  implemented using an optocoupler) so an external pull-up has to be
  added. 10k seems to work. A 10nF capacitor (to ground) might help
  when you experience a noisy signal.
  The port uses 115200 bps 8N1 (3.x and before used 9600 bps).
- A request pin - 5V needs to be applied to this pin to start
  generating output on the TX pin. Applying 3.3V seems to work.
  To disable the P1 output, put the mcu pin in high impedance mode
  (INPUT) instead of pulling to ground.

It is advisable to use an mcu with more than one UART for easy debugging.
MCUs like ESP32 can be powered by the 5V from the P1 port, have plenty of
UART ports. The UART on ESP32 can also be inverted so no external circuitry
is needed.

## Sub meters

In addition to a smart electricity meter, there can be additional
sub meters attached (e.g., gas, water, thermal and sub electricity
meter). These can talk to the main meter using the (wired or wireless)
MBUS protocol to regularly (hourly for 4.x, every 5 minutes for 5.0)
send over their meter readings. Based on the configuration / connection,
each of these subs gets an MBUS identifier (1-4).

In the P1 message, this identifier is used as the second number in the
OBIS identifiers for the fields for the sub. Currently, the code has
the assignment from MBUS identifier to device type hardcoded in
`fields.h`. For the most common configuration of an electricity meter with
a single gas meter as sub, this works straight away. Other
configurations might need changes to `fields.h` to work.

## License

All of the code and documentation in this library is licensed under the
MIT license, with the exception of the examples, which are licensed
under an even more liberal license.
