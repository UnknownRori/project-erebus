#include <iostream>
#include <string>

#include <raylib.h>

#include "./include/erebus.hpp"

#define MAX_INPUT_CHARS 50

bool IsValidCharacter(int key)
{
  return (key >= '0' && key <= '9') || // Numeric digits
         key == '/' || key == '-';
}

bool IsValidShiftCharacter(int key)
{
  return key == '=' || key == '9' || key == '0' || key == '8' || key == '6';
}

int main()
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  char buffer[MAX_INPUT_CHARS + 1] = "\0"; // Initialize name buffer
  int letterCount = 0;
  auto solver = Rori::Math::MathSolver();

  InitWindow(screenWidth, screenHeight, "Project Erebus");

  SetTargetFPS(60);

  f64 result = 0;
  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
    {
      letterCount--;
      buffer[letterCount] = '\0';
    }
    else if (letterCount < MAX_INPUT_CHARS)
    {
      int key = GetKeyPressed();
      if (IsValidShiftCharacter(key) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))
      {
        std::cout << key << IsKeyDown(KEY_LEFT_SHIFT) << std::endl;
        if (key == '=')
        {
          buffer[letterCount] = '+';
          letterCount++;
        }
        else if (key == '9')
        {
          buffer[letterCount] = '(';
          letterCount++;
        }
        else if (key == '0')
        {
          buffer[letterCount] = ')';
          letterCount++;
        }
        else if (key == '8')
        {
          buffer[letterCount] = '*';
          letterCount++;
        }
        else if (key == '6')
        {
          buffer[letterCount] = '^';
          letterCount++;
        }
      }
      else if (IsValidCharacter(key))
      { // Check if a valid key is pressed
        buffer[letterCount] = (char)key;
        letterCount++;
      }
      else if (IsKeyPressed(KEY_ENTER))
      {
        std::cout << buffer << std::endl;
        auto [temp, err] = solver.evaluate(buffer);
        result = temp;
      }
    }

    // Ensure text fits within the input box
    if (MeasureText(buffer, 20) > 760)
    {
      buffer[letterCount - 1] = '\0';
      letterCount--;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Center the input box
    int boxWidth = 760;                        // Width of the input box
    int boxHeight = 30;                        // Height of the input box
    int boxX = (screenWidth - boxWidth) / 2;   // X coordinate to center the input box
    int boxY = (screenHeight - boxHeight) / 2; // Y coordinate to center the input box

    DrawText("Enter Math Expression:", boxX, boxY - 40, 20, DARKGRAY);
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
    DrawText(TextSubtext(buffer, 0, MAX_INPUT_CHARS), boxX + 10, boxY + 10, 20, MAROON);

    DrawText("Result:", boxX, boxY - 100, 20, DARKGRAY);
    std::string result2 = std::to_string(result);
    DrawText(result2.c_str(), boxX, boxY - 140, 20, DARKGRAY);

    EndDrawing();
  }

  CloseWindow();
}
