#pragma once

#include "Camera3d.h"
#include "Audio.h"
#include "ServiceLocator.h"
class Camera3d;



class AudioListener : Camera3d::Listener
{
public:
	public:

    /**
     * Gets the single instance of the audio listener.
     *
     * @return The single instance of the audio listener.
     */
    static AudioListener* getInstance();

    /**
     * Gets the current position of the audio listener.
     *
     * @return The position of the audio listener
     */
    const Vector3& getPosition() const;

    /**
     * Sets the position of the audio source.
     *
     * @param position The position to set the listener to.
     */
    void setPosition(const Vector3& position);

    /**
     * Sets the position of the audio source.
     * 
     * @param x The x coordinate of the position.
     * @param y The y coordinate of the position.
     * @param z The z coordinate of the position.
     */
    void setPosition(float x, float y, float z);

    /**
     * Returns the gain of the audio listener.
     *
     * @return The gain of the audio listener.
     */
    float getVolume() const;

    /**
     * Sets the gain/volume of the audio listener.
     *
     * @param gain The gain/volume of the listener.
     */
    void setVolume(float gain);

    /**
     * Gets the velocity of the audio source.
     *
     * @return The velocity as a vector.
     */
    const Vector3& getVelocity() const;

    /**
     * Sets the velocity of the audio source
     *
     * @param velocity A vector representing the velocity.
     */
    void setVelocity(const Vector3& velocity);

    /**
     * Sets the velocity of the audio source
     * 
     * @param x The x coordinate of the velocity.
     * @param y The y coordinate of the velocity.
     * @param z The z coordinate of the velocity.
     */
    void setVelocity(float x, float y, float z);

    /**
     * Gets the forward orientation vector of the audio listener.
     *
     * @return The forward vector.
     */
    const Vector3& getOrientationForward() const;

    /**
     * Gets the up orientation vector of the audio listener.
     *
     * @return The up vector.
     */
    const Vector3& getOrientationUp() const;

    /**
     * Sets the orientation of the audio listener.
     *
     * @param forward The forward vector.
     * @param up The up vector.
     */
    void setOrientation(const Vector3& forward, const Vector3& up);

    /**
     * Sets the orientation of the audio listener.
     * Vector3(forwardX,forwardY,forwardZ).
     * @param forwardX The x coordinate of the forward vector.
     * @param forwardY The y coordinate of the forward vector.
     * @param forwardZ The z coordinate of the forward vector.
     */
    void setDir(const Vector3 &Dir);
	/**
     * Sets the orientation of the audio listener.
     * 
	 *	Vector3(upX,upY,upZ)
     * @param upX The x coordinate of the up vector.
     * @param upY The y coordinate of the up vector.
     * @param upZ The z coordinate of the up vector.
     */
    void setUP(const Vector3 &Up);
    /**
     * Gets the camera currently associated with the audio listener.
     *
     * @return camera The camera currently associated with the audio listener.
     */
    Camera3d* getCamera() const;

    /**
     * Sets the camera that is associated with the audio listener. This should usually be the current camera.
     *
     * @param camera The camera that is associated with the audio listener
     */
    void setCamera(Camera3d* camera);

private:

    /**
    * Constructor.
    */
    AudioListener();

    /**
    * Destructor.
    */
    ~AudioListener();

    /**
    * @see Camera::Listener::cameraChanged
    */
    void cameraChanged(Camera3d* camera);

    float _volume;
    Vector3 _position;
    Vector3 _velocity;
    Vector3 _up;
	Vector3 _dir;
    Camera3d* _camera;

};


