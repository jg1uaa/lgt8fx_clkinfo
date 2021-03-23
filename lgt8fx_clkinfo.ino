// SPDX-License-Identifier: WTFPL
#include <avr/sfr_defs.h>

#define CLKPR 0x61
#define PMCR  0xf2

// this technique from https://qnighy.hatenablog.com/entry/20091107/1257587259
#define str_2(x)  #x
#define str(x)    str_2(x)

void hex2digit(char *out, uint8_t data)
{
  const static char hex[] = "0123456789ABCDEF";
  *out++ = hex[data >> 4];
  *out++ = hex[data & 0x0f];
  *out = '\0';
}

void display_endis(uint8_t v)
{
  Serial.print(v ? "enabled" : "disabled");
  Serial.print("\n");
}

void setup() {
  Serial.begin(9600);

  Serial.print("* configuration\n");
#if defined(F_CPU)
  Serial.print("#define F_CPU ");
  Serial.print(str(F_CPU));
  Serial.print("\n");
#endif
#if defined(F_OSC)
  Serial.print("#define F_OSC ");
  Serial.print(str(F_OSC));
  Serial.print("\n");
#endif
#if defined(F_DIV)
  Serial.print("#define F_DIV ");
  Serial.print(str(F_DIV));
  Serial.print("\n");
#endif

#if defined(F_CPU)
  Serial.print("F_CPU:");
  Serial.print(F_CPU);
  Serial.print(" ");
#endif
#if defined(F_OSC)
  Serial.print("F_OSC:");
  Serial.print(F_OSC);
  Serial.print(" ");
#endif
#if defined(F_DIV)
  Serial.print("F_DIV:");
  Serial.print(F_DIV);
  Serial.print(" ");
#endif
#if defined(F_CPU) | defined(F_OSC) | defined(F_DIV)
  Serial.print("\n");
#endif
  Serial.print("\n");

  char hex[3];
  uint8_t pmcr = _MMIO_BYTE(PMCR);

  Serial.print("* PMCR(0x");
  hex2digit(hex, pmcr);
  Serial.print(hex);
  Serial.print(")\n");

  Serial.print("internal 32MHz RC-OSC: ");
  display_endis(pmcr & 0x01);
  Serial.print("internal 32kHz RC-OSC: ");
  display_endis(pmcr & 0x02);
  Serial.print("external 400kHz-32MHz OSC: ");
  display_endis(pmcr & 0x04);
  Serial.print("external 32kHz-400kHz OSC: ");
  display_endis(pmcr & 0x08);
  Serial.print("WDT clock source: internal ");
  Serial.print((pmcr & 0x10) ? "32kHz OSC" : "32MHz OSC / 16");
  Serial.print("\n");
  Serial.print("clock source: ");
  switch(pmcr & 0x60) {
    case 0x00: Serial.print("internal 32MHz RC-OSC"); break;
    case 0x20: Serial.print("external 400kHz-32MHz OSC"); break;
    case 0x40: Serial.print("internal 32kHz RC-OSC"); break;
    case 0x60: Serial.print("external 32kHz-400kHz OSC"); break;
    default: Serial.print("unknown"); break;
  }
  Serial.print("\n\n");

  uint8_t clkpr = _MMIO_BYTE(CLKPR);

  Serial.print("* CLKPR(0x");
  hex2digit(hex, clkpr);
  Serial.print(hex);
  Serial.print(")\n");

  Serial.print("prescaler: 1/");
  Serial.print(1 << (clkpr & 0x0f));
  Serial.print("\n");
  Serial.print("PB0 clock output: ");
  display_endis(clkpr & 0x20);
  Serial.print("PE5 clock output: ");
  display_endis(clkpr & 0x40);

  Serial.print("\n");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
