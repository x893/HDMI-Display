#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "HDMI-Display.h"

Touchpanel::Touchpanel()
{
  mouseX = mouseY = 0;
  mouseButtonState = 0;
  mouseZoom = 0;
  axes = 0;
  power = 0;
}

void Touchpanel::on()
{
  power = 1;
}

void Touchpanel::off()
{
  power = 0;
}

void Touchpanel::orientation(uint8_t o)
{
  axes = o;
}

void Touchpanel::setup()
{
}

void Touchpanel::calibration()
{
}

void Touchpanel::mouseButtonDown()
{
  uint16_t x, y;
  
  digitalWrite(LED_2, HIGH);

  if(backlight.screensaverNotify()) // reset screensaver on touch
  {
    return; //backlight was off
  }

  mouseButtonState = 1;

  if(mouseZoom)
  {
    mouseButtonState = 0;
  }

  if(axes & 0x01) //invert x
    x = TOUCHMAX-mouseX;
  else
    x = mouseX;

  if(axes & 0x02) // invert y
    y = TOUCHMAX-mouseY;
  else
    y = mouseY;

  if(axes & 0x04) //swap axes
    SWAP(x, y);

  if(axes & 0x08) //map to screen coordinates
  {
    x = map(x, 0, TOUCHMAX, 0, SCREEN_WIDTH);
    y = map(y, 0, TOUCHMAX, 0, SCREEN_HEIGHT);
  }

  Mouse.moveAbs(x, y, mouseZoom, mouseButtonState);

  #if DEBUG > 0
    Serial.print(mouseX);
    Serial.print(" ");
    Serial.print(mouseY);
    Serial.println(F(" down"));
  #endif
}

void Touchpanel::mouseButtonUp()
{
  uint16_t x, y;

  digitalWrite(LED_2, LOW);

  if(mouseButtonState == 0)
  {
    return;
  }

  mouseButtonState = 0;

  if(axes & 0x01) //invert x
    x = TOUCHMAX-mouseX;
  else
    x = mouseX;

  if(axes & 0x02) // invert y
    y = TOUCHMAX-mouseY;
  else
    y = mouseY;

  if(axes & 0x04) //swap axes
    SWAP(x, y);

  if(axes & 0x08) //map to screen coordinates
  {
    x = map(x, 0, TOUCHMAX, 0, SCREEN_WIDTH);
    y = map(y, 0, TOUCHMAX, 0, SCREEN_HEIGHT);
  }

  Mouse.moveAbs(x, y, 0, 0);

  #if DEBUG > 0
    Serial.print(mouseX);
    Serial.print(" ");
    Serial.print(mouseY);
    Serial.println(F(" up"));
  #endif
}

void Touchpanel::loop()
{
}