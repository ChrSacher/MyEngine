#pragma once


class FpsLimiter 
{
    public:
        FpsLimiter();
        // Initializes the FPS limiter. For now, this is
        // analogous to setMaxFPS
        void init(float maxFPS);

        // Sets the desired max FPS
        void setMaxFPS(float maxFPS);

        void begin();
        // end() will return the current FPS as a float
        float end();
		float getDelta(){return _frameTime;};
    private:
        // Calculates the current FPS
        void calculateFPS();
		//getter
		
        // Variables
        float _fps;
        float _maxFPS;
        float _frameTime;
        unsigned int _startTicks;
};
