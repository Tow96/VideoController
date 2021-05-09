/** pins.h
 * Copyright (c) Jose Tow, 2021
 * Header file that contains the classes utilized to contain which pins are
 * connected to each component for the button matrix and the knobs
 */

#ifndef PINS_H_
  #define PINS_H_
  
  // Knobs
  #define topKnobA 8
  #define topKnobB 7
  #define midKnobA 10
  #define midKnobB 9
  #define botKnobA 13
  #define botKnobB 14
  #define wheelA   11
  #define wheelB   12

  const byte allKnobPins[] = 
  {
    topKnobA, topKnobB,
    midKnobA, midKnobB,
    botKnobA, botKnobB,
    wheelA, wheelB
  };

  // Matrix
  #define rowA 5
  #define rowB 6
  #define rowC 16
  #define rowD 15

  #define col1 17
  #define col2 18
  #define col3 3
  #define col4 4

  const byte rowPins[] = {rowA, rowB, rowC, rowD};
  const byte colPins[] = {col1, col2, col3, col4};

#endif