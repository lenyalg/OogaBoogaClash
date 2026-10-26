#include "input.h"

const float STICK_DEADZONE = 0.2f;
const float TRIGGER_DEADZONE = 0.1f;

const float FASTFALL_FLICK_WINDOW = 0.1f;

void InitPlayerInput(PlayerInput* input, InputType type, int gamepadId) {
  input->type = type;
  input->gamepadId = gamepadId;
  input->isConnected = (type == INPUT_KEYBOARD) ? true : IsGamepadAvailable(gamepadId);
  input->wasDownPressedLastFrame = false;
  input->downInputTimer = 0.0f;
}

void UpdateGamepadConnections(PlayerInput* player1Input, PlayerInput* player2Input) {
    if (player1Input->type == INPUT_GAMEPAD) {
      player1Input->isConnected = IsGamepadAvailable(player1Input->gamepadId);
    }
    
    if (player2Input->type == INPUT_GAMEPAD) {
      player2Input->isConnected = IsGamepadAvailable(player2Input->gamepadId);
    }
}


static bool IsKeyboardActionPressed(bool isP2, InputAction action) {
  if (!isP2) {
    switch (action) {
      case LEFT:   return IsKeyPressed(KEY_A);
      case RIGHT:  return IsKeyPressed(KEY_D);
      case JUMP:   return IsKeyPressed(KEY_W);
      case DOWN:   return IsKeyPressed(KEY_S);
      case ATTACK: return IsKeyPressed(KEY_E);
      default:     return false;
    }
  } else {
    switch (action) {
      case LEFT:   return IsKeyPressed(KEY_K);
      case RIGHT:  return IsKeyPressed(KEY_SEMICOLON);
      case JUMP:   return IsKeyPressed(KEY_O);
      case DOWN:   return IsKeyPressed(KEY_L);
      case ATTACK: return IsKeyPressed(KEY_I);
      default:     return false;
    }
  }
}

static bool IsKeyboardActionDown(bool isP2, InputAction action) {
    if (!isP2) {
        switch (action) {
            case LEFT:   return IsKeyDown(KEY_A);
            case RIGHT:  return IsKeyDown(KEY_D);
            case JUMP:   return IsKeyDown(KEY_W);
            case DOWN:   return IsKeyDown(KEY_S);
            case ATTACK: return IsKeyDown(KEY_E);
            default:     return false;
        }
    } else {
        switch (action) {
            case LEFT:   return IsKeyDown(KEY_K);
            case RIGHT:  return IsKeyDown(KEY_SEMICOLON);
            case JUMP:   return IsKeyDown(KEY_O);
            case DOWN:   return IsKeyDown(KEY_L);
            case ATTACK: return IsKeyDown(KEY_I);
            default:     return false;
        }
    }
}

static bool IsGamepadActionPressed(int gamepadId, InputAction action) {
    if (!IsGamepadAvailable(gamepadId)) return false;
    
    switch (action) {
        case LEFT: {
            float axisX = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
            return (axisX < -STICK_DEADZONE && GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X) >= -STICK_DEADZONE) ||
                   IsGamepadButtonPressed(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        }
        case RIGHT: {
            float axisX = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
            return (axisX > STICK_DEADZONE && GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X) <= STICK_DEADZONE) ||
                   IsGamepadButtonPressed(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        }
        case JUMP:
            return IsGamepadButtonPressed(gamepadId, GAMEPAD_BUTTON_RIGHT_FACE_DOWN); 
        case DOWN: {
            float axisY = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_Y);
            return (axisY > STICK_DEADZONE) ||
                   IsGamepadButtonPressed(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        }
        case ATTACK:
            return IsGamepadButtonPressed(gamepadId, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        default:
            return false;
    }
}

static bool IsGamepadActionDown(int gamepadId, InputAction action) {
    if (!IsGamepadAvailable(gamepadId)) return false;
    
    switch (action) {
        case LEFT: {
            float axisX = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
            return (axisX < -STICK_DEADZONE) || 
                   IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        }
        case RIGHT: {
            float axisX = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
            return (axisX > STICK_DEADZONE) || 
                   IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        }
        case JUMP:
            return IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        case DOWN: {
            float axisY = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_Y);
            return (axisY > STICK_DEADZONE) || 
                   IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        }
        case ATTACK:
            return IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        default:
            return false;
    }
}

bool IsActionPressed(Character* player, InputAction action) {
    if (player->input.type == INPUT_KEYBOARD) {
        return IsKeyboardActionPressed(player->isP2, action);
    } else {
        return IsGamepadActionPressed(player->input.gamepadId, action);
    }
}

bool IsActionDown(Character* player, InputAction action) {
    if (player->input.type == INPUT_KEYBOARD) {
        return IsKeyboardActionDown(player->isP2, action);
    } else {
        return IsGamepadActionDown(player->input.gamepadId, action);
    }
}


bool IsDownFlicked(Character* player) {
  bool isDownNow = IsActionDown(player, DOWN);
  bool wasDownBefore = player->input.wasDownPressedLastFrame;
  bool justPressed = isDownNow && !wasDownBefore;
  player->input.wasDownPressedLastFrame = isDownNow;
  return justPressed;
}

int GetDashDirection(Character* player) {
    int dir = 0;
    if (player->input.type == INPUT_KEYBOARD) {
        if (!player->isP2) {
            if (IsKeyPressed(KEY_Q)) { 
                if (IsActionDown(player, LEFT)) dir = -1;
                else if (IsActionDown(player, RIGHT)) dir = 1;
                else dir = player->isFacingRight ? 1 : -1;
            }
        } else {
            if (IsKeyPressed(KEY_P)) {
                if (IsActionDown(player, LEFT)) dir = -1;
                else if (IsActionDown(player, RIGHT)) dir = 1;
                else dir = player->isFacingRight ? 1 : -1;
            }
        }
    } else {
        int gid = player->input.gamepadId;
        if (IsGamepadAvailable(gid)) {
            if (IsGamepadButtonPressed(gid, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
                if (IsActionDown(player, LEFT)) dir = -1;
                else if (IsActionDown(player, RIGHT)) dir = 1;
                else dir = player->isFacingRight ? 1 : -1;
            }
        }
    }
    return dir;
}