#include "Camera3d.h"
#include "ServiceLocator.h"
#include "SpecComponent.h"
const float DEG2RAD = 3.141593f / 180;
const float RAD2DEG = 180 / 3.141593f;

const static float STEP_SCALE = 0.1f;
const float mouseSpeed = 0.01f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 50;

Camera3d* Camera3d::currentCamera;

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
Camera3d::Camera3d(Vector3 &Pos, float fov, int width, int height, float zNear, float zFar) 
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
	currentCamera = this;
	cameraScript = NULL;
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

void Camera3d::strafeleft(float distance)
{
	Vector3 Left = dir.cross(up);
	Left.normalize();
	if (distance == 0)
	{
		Left *= cameraspeed;
		pos += Left;
	}
	else
	{
		Left *= distance;
		pos += Left;
	}

}
void Camera3d::straferight(float distance)
{
	Vector3 Right = up.cross(dir);
	Right.normalize();
	if (distance == 0)
	{
	Right *= cameraspeed;
	pos += Right;
	}
	else
	{
		Right *= distance;
		pos += Right;
	}
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
	float X, Y;
	X = (float)x / (float)windowWidth * 1000.0f;
	Y = (float)y / (float)windowHeight * 1000.0f;

	if (ignore) return;

	AngleH -= mouseSpeed  * float(500.0f - X);
	if (AngleH > 360.0f * DEG2RAD) AngleH -= 360 * DEG2RAD;
	if (AngleH < 0.0f * DEG2RAD) AngleH += 360 * DEG2RAD;

	AngleV -= mouseSpeed * float(500.0f - Y);
	if (AngleV > 360.0f* DEG2RAD) AngleV -= 360 * DEG2RAD;
	if (AngleV < 0.0f* DEG2RAD) AngleV += 360 * DEG2RAD;
	if (AngleV * RAD2DEG < 270.0f && AngleV* RAD2DEG > 180.0f) AngleV = 270.0f * DEG2RAD;
	if (AngleV * RAD2DEG> 90.0f && AngleV * RAD2DEG < 180.0f) AngleV = 90.0f* DEG2RAD;
	Update();
}

void Camera3d::onMouse(Vector2 c) { OnMouse(c.x, c.y); }
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

	for (std::list<Camera3d::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
	{
		Camera3d::Listener* listener = (*itr);
		listener->cameraChanged(this);
	}
}

void Camera3d::addListener(Camera3d::Listener* listener)
{
	_listeners.push_back(listener);
}

void Camera3d::removeListener(Camera3d::Listener* listener)
{

		for (std::list<Camera3d::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
		{
			if ((*itr) == listener)
			{
				_listeners.erase(itr);
				break;
			}
		}
}
void Camera3d::setScript(ComponentPosition* SeScript)
{
	if (cameraScript != NULL) ServiceLocator::getCM().deleteComponent(cameraScript);
	cameraScript = SeScript;
	ScriptCreated(&cameraScript->getT<ScriptComponent*>()->script);
}

void Camera3d::setScript(std::string Path)
{
	if (cameraScript != NULL) ServiceLocator::getCM().deleteComponent(cameraScript);
	cameraScript = ServiceLocator::getCM().createScript(Path);
	ScriptCreated(&cameraScript->getT<ScriptComponent*>()->script);
}
ComponentPosition* Camera3d::getScript()
{
	return cameraScript;
}

void Camera3d::ScriptCreated(SeScript* SeScript)
{
	ChaiScript& script = SeScript->getState();
	script.add(user_type<Camera3d>(), "Camera3d");
	script.add_global(var(currentCamera), "Camera");
	script.add(fun(&Camera3d::setPos), "setPos");
	script.add(fun(&Camera3d::getPos), "getPos");
	script.add(fun(&Camera3d::setDir), "setDir");
	script.add(fun(&Camera3d::getDir), "getDir");
	script.add(fun(&Camera3d::setUp), "setUp");
	script.add(fun(&Camera3d::getUp), "getUp");
	script.add(fun(&Camera3d::setFov),"setFov");
	script.add(fun(&Camera3d::getFov),"getFov");
	script.add(fun(&Camera3d::onMouse),"onMouse");
	script.add(fun(&Camera3d::getSize),"getSize");
	script.add(fun(&Camera3d::moveforward),"moveForward");
	script.add(fun(&Camera3d::movebackward),"moveBackward");
	script.add(fun(&Camera3d::raise),"raise");
	script.add(fun(&Camera3d::sink),"sink");
	script.add(fun(&Camera3d::strafeleft),"strafeleft");
	script.add(fun(&Camera3d::straferight),"straferight");
}
