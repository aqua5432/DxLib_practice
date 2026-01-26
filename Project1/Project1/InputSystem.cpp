#include "InputSystem.h"
#include "DxLib.h"
#include "Sub.h"   // Keyグローバルが定義されてるやつ

InputState InputSystem::Update()
{
    InputState in{};

    in.dash = Key.input[KEY_INPUT_F] > 0;
    in.jump = Key.input[KEY_INPUT_W] == 1 || Key.input[KEY_INPUT_UP] == 1;
    in.moveRight = Key.input[KEY_INPUT_D] > 0 || Key.input[KEY_INPUT_RIGHT] > 0;
    in.moveLeft = Key.input[KEY_INPUT_A] > 0 || Key.input[KEY_INPUT_LEFT] > 0;
    in.exit = Key.input[KEY_INPUT_ESCAPE] > 0;

    return in;
}
