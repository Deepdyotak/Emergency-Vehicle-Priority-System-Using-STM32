#include "traffic_control.h"
#include "main.h"

uint8_t read_emergency_lane()
{
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) return 1; // Lane 1
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) return 2; // Lane 2
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) return 3; // Lane 3
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) return 4; // Lane 4
  return 0;
}

// Turn off all lights
void turn_all_red()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);   // Lane C red
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   // Lane E red
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  // Lane F red
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);   // Lane D red

  // Turn off all green and yellow
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
}

void run_emergency_lane(uint8_t lane)
{
  turn_all_red(); // First, ensure all lanes are stopped

  switch (lane)
  {
    case 1:
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); // Red OFF
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   // Green ON
      break;
    case 2:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
      break;
    case 3:
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
      break;
    case 4:
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
      break;
  }
}

void run_lights(char ch)
{
  turn_all_red();  // Reset all lights

  if (ch == 'c') {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);  // Red OFF
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);    // Green ON
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);    // Yellow ON
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
  }
  else if (ch == 'e') {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
  }
  else if (ch == 'f') {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
  }
  else if (ch == 'd') {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
  }
}

void normal_rotate()
{
  char arr[] = {'c', 'e', 'f', 'd'};

  for (int i = 0; i < 4; i++) {
    uint8_t emergency_lane = read_emergency_lane();

    if (emergency_lane != 0) {
      // Emergency overrides normal rotation
      while ((emergency_lane = read_emergency_lane()) != 0) {
        run_emergency_lane(emergency_lane);
        HAL_Delay(200); // Short delay to avoid CPU lock
      }
    } else {
      run_lights(arr[i]);
      HAL_Delay(100);  // Slight delay between lanes
    }
  }
}
