===========
Alarm clock
===========

Write an 8051 program which implements an alarm clock. This project will
have two phases, the first of which is described here. The alarm clock
runs all the time, with the current time displayed on the four 7-segment
displays. In this first phase your clock needs to:
  1. use timers to keep time updated to the second
  2. switch between 12 hour and 24 hour modes based switch bank.0 (0==12
   hr, 1==24hr)
  3. if you are in 12 hour mode, use the decimal point on the last
   7-segment display to indicate AM or PM (off = AM, on == PM)
  4. switch between display of hours:minutes and minutes:seconds based on
   switch bank.1 (0==hours:minutes, 1==minutes:seconds)
  5. keep the decimal point of the second 7-segment display lit as a
   visual separator

AM/PM should remain indicated regardless of which display mode you are
using.

You do not (yet) need to be able to:
  - set the time (pick a starting value)
  - set an alarm time
  - perform an alarm
