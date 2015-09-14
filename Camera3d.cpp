#include "Camera3d.h"
const float DEG2RAD = 3.141593f / 180;
const float RAD2DEG = 180 / 3.141593f;

const static float STEP_SCALE = 0.1f;
const float mouseSpeed = 0.2f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 50;

Ray Camera3d::getDirClick(int x, int y)
{
	float x2 = (2.0f *x) / windowWidth - 1.0f;
	float y2 = 1.0f - (2.0f * y) / windowHeight;
	x2 = -x2; //for whatever reason/ it works
	y2 = -y2;
	Vector4 ray_clip = Vector4(x2, y2, -1.0, 1.0);
	Matrix4 temp1 = projectionMatrix;
	Vector4 ray_eye = temp1.invertGeneral() * ray_clip;
	ray_eye.z = -1.0;
	ray_eye.w = 0.0;
	Vector4 ray_wor = ((Matrix4().identity().lookAt(pos, pos - dir, up)).invert() * ray_eye);
	Vector3 returnV = Vector3(ray_wor.x, ray_wor.y, ray_wor.z);
	// don't forget to normalise the vector at some point
	returnV = returnV.normalize();
	returnV.conjugate(); // invert
	return Ray(pos, returnV);

}
Camera3d::Camera3d(Vector3 Pos, float fov, int width, int height, float zNear, float zFar) :_listeners(NULL), cameraScript(NULL)
{
	projectionMatrix = Matrix4().perspective(fov, width / height, zNear, zFar);
	pos = Pos;
	up = Vector3(0, 1, 0);
	up.normalize();
	dir = Vector3(1, 0, 0);
	dir.normalize();
	this->zNear = zNear;
	this->zFar = zFar;
	windowWidth = width;
	windowHeight = height;
	cameraspeed = 1;
	this->fov = fov;
	init();
}

void Camera3d::setFov(float Nfov)
{
	fov = Nfov;
	projectionMatrix = Matrix4().perspective(fov, windowWidth / windowHeight, zNear, zFar);
}
void Camera3d::update(Shader *shader)
{


	shader->matrices.view = GetViewProjection();
	//shader->setUniform("viewMatrix",shader->matrices.view);
	shader->setUniform("eyePos", pos);

}
Matrix4& Camera3d::GetViewProjection()
{
	static Matrix4 oldView;
	viewMatrix = (Matrix4().identity().lookAt(pos, pos - dir, up));
	viewProjection = projectionMatrix * viewMatrix;
	if (viewProjection != oldView)
	{
		cameraChanged();
	};
	oldView = viewProjection;
	return viewProjection;
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
			AngleH = 360.0f - (asin(HTarget.z) * RAD2DEG);
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
			AngleH = (asin(-HTarget.z) * RAD2DEG);
		}
		else
		{
			AngleH = 90.0f + (asin(-HTarget.z) * RAD2DEG);
		}
	}

	AngleV = -(asin(dir.y) * RAD2DEG);
	mousePos.x = 500;
	mousePos.y = 500;
}
void Camera3d::updateProjectionMatrix(float fov, int width, int height, float zNear, float zFar)
{


	projectionMatrix = Matrix4().identity().perspective(fov, width / height, zNear, zFar);
	windowWidth = width;
	windowHeight = height;
}

void Camera3d::moveforward(float distance)
{
	if (distance == 0)
	{
		pos += (dir * cameraspeed);
	}
	else
	{
		pos += (dir * distance);
	}
}
void Camera3d::movebackward(float distance)
{
	if (distance == 0)
	{
		pos -= (dir * cameraspeed);
	}
	else
	{
		pos -= (dir * distance);
	}
}
void Camera3d::raise(float distance)
{
	if (distance == 0)
	{
		pos += Vector3(0, 1, 0) * cameraspeed;
	}
	else
	{
		pos += Vector3(0, 1, 0) * distance;
	}
}
void Camera3d::sink(float distance)
{
	if (distance == 0)
	{
		pos -= Vector3(0, 1, 0) * cameraspeed;
	}
	else
	{
		pos -= Vector3(0, 1, 0) * distance;
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

void Camera3d::OnMouse(int x, int y, bool ignore)
{

	mousePos.x = x;
	mousePos.y = y;
	x = (float)x / windowWidth * 1000;
	y = (float)y / windowHeight * 1000;

	if (ignore) return;

	AngleH -= mouseSpeed  * float(500 - x);
	if (AngleH > 360) AngleH -= 360;
	if (AngleH < 0) AngleH += 360;

	AngleV -= mouseSpeed * float(500 - y);
	if (AngleV > 360) AngleV -= 360;
	if (AngleV < 0) AngleV += 360;

	if (AngleV < 270 && AngleV > 180)AngleV = 270;
	if (AngleV + 180 > 270 && AngleV  < 180)AngleV = 90;
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
void Camera3d::cameraChanged()
{
	if (_listeners == NULL)
		return;

	for (std::list<Camera3d::Listener*>::iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
	{
		Camera3d::Listener* listener = (*itr);
		listener->cameraChanged(this);
	}
}

void Camera3d::addListener(Camera3d::Listener* listener)
{
	if (_listeners == NULL)
		_listeners = new std::list<Camera3d::Listener*>();
	if (listener) _listeners->push_back(listener);
}

void Camera3d::removeListener(Camera3d::Listener* listener)
{

	if (_listeners)
	{
		for (std::list<Camera3d::Listener*>::iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
		{
			if ((*itr) == listener)
			{
				_listeners->erase(itr);
				break;
			}
		}
	}
}
void Camera3d::setScript(LuaScript* script)
{
	if (cameraScript != NULL) ServiceLocator::getLua().deleteScript(cameraScript);
	cameraScript = script;
}

void Camera3d::setScript(std::string Path)
{
	if (cameraScript != NULL) ServiceLocator::getLua().deleteScript(cameraScript);
	cameraScript = ServiceLocator::getLua().createScript(Path);
}
LuaScript* Camera3d::getScript()
{
	return cameraScript;
}