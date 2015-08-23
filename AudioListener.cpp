#include "AudioListener.h"

AudioListener::AudioListener()
    : _volume(1.0f), _camera(NULL)
{
}

AudioListener::~AudioListener()
{
	// Call setCamera() to release camera and cause transform listener
	// to be removed.
	setCamera(NULL);
}

AudioListener* AudioListener::getInstance()
{
	static AudioListener listener;
    return &listener;
}

float AudioListener::getVolume() const 
{ 
    return _volume; 
}

void AudioListener::setVolume(float gain)
{
    _volume = gain;
}

const Vector3& AudioListener::getPosition() const 
{ 
    return _position; 
}

void AudioListener::setPosition(const Vector3& position)
{
    _position = position;
}

void AudioListener::setPosition(float x, float y, float z)
{
    _position.set(x, y, z);
}

const Vector3& AudioListener::getVelocity() const 
{ 
    return _velocity; 
}

void AudioListener::setVelocity(const Vector3& velocity)
{
    _velocity = velocity;
}

void AudioListener::setVelocity(float x, float y, float z)
{
    _velocity.set(x, y, z);
}

const Vector3& AudioListener::getOrientationForward() const 
{ 
    return _dir; 
}

const Vector3& AudioListener::getOrientationUp() const 
{ 
    return _up; 
}

void AudioListener::setOrientation(const Vector3& forward, const Vector3& up)
{
    _dir.x = forward.x;
    _dir.y = forward.y;
    _dir.z = forward.z;

    _up.x = up.x;
    _up.y = up.y;
    _up.z = up.z;
}

Camera3d* AudioListener::getCamera() const
{
    return _camera;
}

void AudioListener::setCamera(Camera3d* camera)
{
    if (_camera == camera)
        return;
	if(_camera != NULL)
	{
		_camera->removeListener(this);

	}
	if(camera != NULL)
	{
		camera->addListener(this);
	}
    _camera = camera;
  
}

void AudioListener::cameraChanged(Camera3d* camera)
{
    if (_camera != camera)
        setCamera(camera);
   
    if (_camera)
	{
            setPosition(camera->getPos());
            setOrientation(camera->getDir(), camera->getUp());
			//setVelocity(camera->getVelocity);
       
    }
	else
    {
        setPosition(Vector3());
        setOrientation(Vector3(0,1,0), -Vector3(0,0,-1));
		setVelocity(Vector3());
    }
	ServiceLocator::getAudio().update(_position,_dir,_velocity,_up);
}
