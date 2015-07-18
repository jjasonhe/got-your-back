# got-your-back
Got Your Back is an ergonomics-focused smart chair concept created by Dana Abusalah and Jiacheng Jason He for the Texas Instruments 2015 DIY with TI: Intern Edition Challenge. This repo is for the MSP432 LaunchPad hardware-side code, built in Energia 15.

## What do I do
You must have Energia 15 or 16, with the proper drivers installed. This code leverages the MultiTasking (MT) feature using TI-RTOS for the MSP432, so we don't think it will work if ported to a different LaunchPad.

Download or fork this repo. Keep all files in one folder. Open ```CensoRed.ino``` in Energia, and your IDE should open with all the files (sans README) as tabs.

That's it! Just verify to make sure it works (it should...), and flash it onto your board.

## What do you need
To actually use this code in a meaningful way, you'll need some hardware.

- Texas Instruments MSP-EXP432P401R LaunchPad
- Texas Instruments SimpleLink WiFi CC3100 BoosterPack
- Seeed Studio Grove LaunchPad Starter Kit
- Seeed Studio Ultrasonic Ranger (additional)
- A mesh chair lumbar support

## What does this do
This code preps your LaunchPad to read ultrasonic sensor data every user-determined amount of minutes, check to see if it fits the parameter, and responds by publishing information via PubSubClient WiFi.

###*For more information, please visit [**our page on Hackster.io**](https://www.hackster.io/creativiteam/got-your-back)!*