const cspeed = 30;
function update()
if(input:isKeyDown(SDLK_w)) camera:moveCameraForward(cspeed * time.delta);
if(input:isKeyDown(SDLK_s)) camera:moveCameraBackward(cspeed * time.delta);
if(input:isKeyDown(SDLK_a)) camera:moveCameraMoveLeft(cspeed * time.delta);
if(input:isKeyDown(SDLK_d)) camera:moveCameraMoveRight(cspeed * time.delta);
if(input:isKeyDown(SDLK_q)) camera:moveCameraMoveUp(cspeed * time.delta);
if(input:isKeyDown(SDLK_e)) camera:moveCameraMoveDown(cspeed * time.delta));
end