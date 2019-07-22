/*
 * CellularAutomata.cpp
 *
 *  Created on: Jul 20, 2019
 *      Author: raffc
 */

#include "CellularAutomata.h"
#include <FastLED.h>
#include "cube.h"

CellularAutomata::CellularAutomata() {
  duration = 20000;

  lastUpdateTime = 0;
  waitTime = 100;

  rhs = true;

  // initialize rules
  memset(survival, false, 27);
  memset(birth, false, 27);

  // some decent rules, if rules aren't being randomized
  maxCellStates = 10;
  for (byte i = 4; i < 8; i++)
    survival[i] = true;
  for (byte i = 6; i < 9; i++)
    birth[i] = true;
}

void CellularAutomata::randomizeRules() {
  memset(survival, false, 27);
  memset(birth, false, 27);

  maxCellStates = 2 + random(8);
  for (byte i = 0; i < 27; i++) {
    if (random(3) == 0)
      survival[i] = true;
    if (random(3) == 0)
      birth[i] = true;
  }
}

void CellularAutomata::initialize() {
  memset(cells, 0, 512);

  randomizeRules();

  // initialize some cells
  for (byte i = 2; i <= 5; i++) {
    for (byte j = 2; j <= 5; j++) {
      for (byte k = 2; k <= 5; k++) {
        // randomly set cell to be alive
        if (random(2) == 0) {
          cells[getIndex(i, j, k)] = 1;
        }
      }
    }
  }

  rhs = true;
}

byte unpackPreviousValue(byte cell, bool rhs) {
  if (rhs) {
    return 0b00001111 & cell;
  }

  return cell >> 4;
}

byte unpackNextValue(byte cell, bool rhs) {
  return unpackPreviousValue(cell, !rhs);
}

void CellularAutomata::tick() {
  if (millis() - lastUpdateTime <= waitTime)
    return;

  lastUpdateTime = millis();

  FastLED.clear();

  bool allDead = true; // as we update, check if all the cells have died

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      for (byte k = 0; k < 8; k++) {
        byte neighbors = 0;

        // scan the ol' Moore neighborhood
        for (byte l = max(0, i-1); l < min(8, i+2); l++) {
          for (byte m = max(0, j-1); m < min(8, j+2); m++) {
            for (byte n = max(0, k-1); n < min(8, k+2); n++) {
              // ignore the center cell when computing its neighbors
              if (n == k && m == j && l == i)
                continue;

              byte cellState = unpackPreviousValue(cells[getIndex(l, m, n)], rhs);
              if (cellState > 0) {
                neighbors++;
              }
            }
          }
        }

        byte oldValue = unpackPreviousValue(cells[getIndex(i, j, k)], rhs);
        byte newValue = oldValue;

        // if this cell is already alive
        if (oldValue > 0) {
          if (survival[neighbors])
            continue;

          // if the cell doesn't have the right number of cells around it,
          // advance its state until it dies
          newValue += 1;

          if (newValue >= maxCellStates)
            newValue = 0;
        }
        // if there is no living cell here, check if one should be born
        else if (birth[neighbors]) {
          newValue = 1;
        }

        // if there is a cell here, draw it
        if (newValue > 0) {
          leds[getIndex(i, j, k)] = CHSV(127 + 127 * newValue / maxCellStates, 255, 127);

          allDead = false;
        }

        // pack the values
        if (rhs) {
          cells[getIndex(i, j, k)] = (newValue << 4) | oldValue;
        } else {
          cells[getIndex(i, j, k)] = (oldValue << 4) | newValue;
        }
      }
    }
  }

  rhs = !rhs;

  if (allDead) {
    initialize();
  }
}
