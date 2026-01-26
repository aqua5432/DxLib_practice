#pragma once

struct InputState {
    bool dash;
    bool jump;
    bool moveRight;
    bool moveLeft;
    bool exit;
};

class InputSystem {
public:
    InputState Update();
};
