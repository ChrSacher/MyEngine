#include "Camera3d.h"
const float DEG2RAD = 3.141593f / 180;
const float RAD2DEG = 180 / 3.141593f;

const static float STEP_SCALE = 0.1f;
const float mouseSpeed = 0.2f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 50;

Camera3d::Camera3d(Vector3 Pos,float fov,int width,int height,float zNear,float zFar)
{
	perspectiveMatrix=Matrix4().perspective(fov,width/height,zNear,zFar);
	pos = Pos;
	up = Vector3(0,1,0);
	up.normalize();
	dir = Vector3(1,0,0);
	dir.normalize();
	this->zNear = zNear;
	this->zFar = zFar;
	windowWidth = width;
	windowHeight = height;
	cameraspeed=1;
	this->fov = fov;
	init();
}

void Camera3d::setFov(float Nfov)
{
	fov = Nfov;
	perspectiveMatrix=Matrix4().perspective(fov,windowWidth/windowHeight,zNear,zFar);
}
void Camera3d::update(Shader *shader)
{
	shader->matrices.view = GetViewProjection();
	//shader->setUniform("viewMatrix",shader->matrices.view);
	shader->setUniform("eyePos",pos);
}
Matrix4& Camera3d::GetViewProjection() 
{
	viewMatrix = perspectiveMatrix * (Matrix4().identity().lookAt(pos,pos - dir,up));
	return viewMatrix;
}

Camera3d::~Camera3d(void)
{

}

void Camera3d::init()
{
	Vector3 HTarget(dir.x, 0.0, dir.z);
    HTarget.normalize();
    
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            AngleH = 360.0f -(asin(HTarget.z) * RAD2DEG);
        }
        else
        {
            AngleH = 180.0f + (asin(HTarget.z) * RAD2DEG);
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            AngleH =(asin(-HTarget.z) * RAD2DEG);
        }
        else
        {
            AngleH = 90.0f + (asin(-HTarget.z) * RAD2DEG);
		}
    }
    
    AngleV = -(asin(dir.y) * RAD2DEG);
	mousePos.x  = (float)windowWidth / 2;
    mousePos.y  = (float)windowHeight / 2;
}
void Camera3d::updatePerspectiveMatrix(float fov,int width,int height,float zNear,float zFar)
{


	perspectiveMatrix=Matrix4().identity().perspective(fov,width/height,zNear,zFar);
	windowWidth = width;
	windowHeight = height;
}

void Camera3d::moveforward(float distance)
{
	if(distance==0)
	{
		 pos += (dir * cameraspeed);
	}
	else
	{
		 pos += (dir * distance);
	}
}
void Camera3d::movebackward(float distance )
{
	if(distance==0)
	{
		 pos -= (dir * cameraspeed);
	}
	else
	{
		 pos -= (dir * distance);
	}
}
void Camera3d::raise(float distance )
{
	if(distance==0)
	{
		 pos += Vector3(0,1,0) * cameraspeed;
	}
	else
	{
		 pos += Vector3(0,1,0) * distance;
	}
}
void Camera3d::sink(float distance )
{
	if(distance==0)
	{
		 pos -= Vector3(0,1,0) * cameraspeed;
	}
	else
	{
		 pos -= Vector3(0,1,0) * distance;
	}

}
void Camera3d::turnright()
{
	
}
void Camera3d::turnleft()
{
	
}

void Camera3d::strafeleft()
{
	Vector3 Left = dir.cross(up);
       Left.normalize();
        Left *= cameraspeed;
        pos += Left;

}
void Camera3d::straferight()
{
	Vector3 Right = up.cross(dir);
     Right.normalize();
     Right *= cameraspeed;
     pos += Right;
}

Vector3 Camera3d::getDir()
{
	return dir;
}
Vector3 Camera3d::getPos()
{
	return pos;
}
Vector3 Camera3d::getUp()
{
	return up;
}

void Camera3d::OnMouse(int x, int y)
{
    const int DeltaX = x - mousePos.x;
    const int DeltaY = y - mousePos.y;

    mousePos.x = (float)x;
    mousePos.y = (float)y;

	
    AngleH -= mouseSpeed  * float(windowWidth/2 - x );
	if(AngleH > 360) AngleH -= 360;
	if(AngleH < 0) AngleH += 360;

	AngleV   -= mouseSpeed * float( windowHeight/2 - y );
	if(AngleV > 360) AngleV -= 360;
	if(AngleV < 0) AngleV += 360;

	if(AngleV < 270 && AngleV > 180)AngleV = 270;
	if(AngleV + 180 > 270 && AngleV  < 180)AngleV = 90;
    Update();
}

void Camera3d::Update()
{
    const Vector3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3 View(1.0f, 0.0f, 0.0f);
    View.rotate(AngleH, Vaxis);
    View.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3 Haxis = Vaxis.cross(View);
    Haxis.normalize();
    View.rotate(AngleV, Haxis);
       
    dir = View;
    dir.normalize();

    up = dir.cross(Haxis);
    up.normalize();
}