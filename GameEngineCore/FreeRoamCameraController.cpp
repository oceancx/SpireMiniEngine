#include "FreeRoamCameraController.h"
#include "Engine.h"

namespace GameEngine
{
	using namespace VectorMath;

	void FreeRoamCameraControllerActor::FindTargetCamera()
	{
		auto actor = level->FindActor(targetCameraName);
		if (actor && actor->GetEngineType() == EngineActorType::Camera)
			targetCamera = (CameraActor*)actor;
	}

	bool FreeRoamCameraControllerActor::ParseField(CoreLib::Text::TokenReader & parser, bool & isInvalid)
	{
		if (Actor::ParseField(parser, isInvalid))
			return true;
		if (parser.LookAhead("TargetCamera"))
		{
			parser.ReadToken();
			targetCameraName = parser.ReadStringLiteral();
			return true;
		}
		else if (parser.LookAhead("Speed"))
		{
			parser.ReadToken();
			cameraSpeed = (float)parser.ReadDouble();
			return true;
		}
		else if (parser.LookAhead("TurnPrecision"))
		{
			parser.ReadToken();
			turnPrecision = (float)parser.ReadDouble();
			return true;
		}
		return false;
	}
	void FreeRoamCameraControllerActor::OnLoad()
	{
		Actor::OnLoad();

		Engine::Instance()->GetInputDispatcher()->BindActionHandler("MoveForward", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveForward));
		Engine::Instance()->GetInputDispatcher()->BindActionHandler("MoveRight", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveRight));
		Engine::Instance()->GetInputDispatcher()->BindActionHandler("MoveUp", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveUp));
		Engine::Instance()->GetInputDispatcher()->BindActionHandler("TurnRight", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::TurnRight));
		Engine::Instance()->GetInputDispatcher()->BindActionHandler("TurnUp", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::TurnUp));
		Engine::Instance()->GetInputDispatcher()->BindActionHandler("dumpcam", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::DumpCamera));

	}
	void FreeRoamCameraControllerActor::OnUnload()
	{
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("MoveForward", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveForward));
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("MoveRight", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveRight));
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("MoveUp", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::MoveUp));
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("TurnRight", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::TurnRight));
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("TurnUp", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::TurnUp));
		Engine::Instance()->GetInputDispatcher()->UnbindActionHandler("dumpcam", ActionInputHandlerFunc(this, &FreeRoamCameraControllerActor::DumpCamera));

	}
	EngineActorType FreeRoamCameraControllerActor::GetEngineType()
	{
		return EngineActorType::UserController;
	}
	bool FreeRoamCameraControllerActor::MoveForward(const CoreLib::String & /*axisName*/, ActionInput input)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			Vec3 moveDir = -Vec3::Create(targetCamera->GetLocalTransform().values[2], targetCamera->GetLocalTransform().values[6], targetCamera->GetLocalTransform().values[10]);
			float dTime = Engine::Instance()->GetTimeDelta(EngineThread::GameLogic);
			targetCamera->SetPosition(targetCamera->GetPosition() + moveDir * (input.AxisValue * dTime * cameraSpeed));
			return true;
		}
		return false;
	}
	bool FreeRoamCameraControllerActor::MoveRight(const CoreLib::String & /*axisName*/, ActionInput input)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			Vec3 moveDir = Vec3::Create(targetCamera->GetLocalTransform().values[0], targetCamera->GetLocalTransform().values[4], targetCamera->GetLocalTransform().values[8]);
			float dTime = Engine::Instance()->GetTimeDelta(EngineThread::GameLogic);
			targetCamera->SetPosition(targetCamera->GetPosition() + moveDir * (input.AxisValue * dTime * cameraSpeed));
			return true;
		}
		return false;
	}
	bool FreeRoamCameraControllerActor::MoveUp(const CoreLib::String & /*axisName*/, ActionInput input)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			Vec3 moveDir = Vec3::Create(targetCamera->GetLocalTransform().values[1], targetCamera->GetLocalTransform().values[5], targetCamera->GetLocalTransform().values[9]);
			float dTime = Engine::Instance()->GetTimeDelta(EngineThread::GameLogic);
			targetCamera->SetPosition(targetCamera->GetPosition() + moveDir * (input.AxisValue * dTime * cameraSpeed));
			return true;
		}
		return false;
	}
	bool FreeRoamCameraControllerActor::TurnRight(const CoreLib::String & /*axisName*/, ActionInput input)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			float dTime = Engine::Instance()->GetTimeDelta(EngineThread::GameLogic);
			targetCamera->SetYaw(targetCamera->GetYaw() + input.AxisValue * dTime * turnPrecision);
			return true;
		}
		return false;
	}
	bool FreeRoamCameraControllerActor::TurnUp(const CoreLib::String & /*axisName*/, ActionInput input)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			float dTime = Engine::Instance()->GetTimeDelta(EngineThread::GameLogic);
			targetCamera->SetPitch(targetCamera->GetPitch() - input.AxisValue * dTime * turnPrecision);
			return true;
		}
		return false;
	}
	bool FreeRoamCameraControllerActor::DumpCamera(const CoreLib::String & /*axisName*/, ActionInput /*input*/)
	{
		FindTargetCamera();
		if (targetCamera)
		{
			Print("orientation [%.3f %.3f %.3f]\nposition [%.2f %.2f %.2f]\n", targetCamera->GetYaw(), targetCamera->GetPitch(), targetCamera->GetRoll(),
				targetCamera->GetPosition().x, targetCamera->GetPosition().y, targetCamera->GetPosition().z);
		}
		return true;
	}
}
