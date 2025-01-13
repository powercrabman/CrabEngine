#pragma once
#include <box2d/box2d.h>

#include "IPlugin.h"

namespace crab
{
	class Scene;

	enum class ePhysicMixRule
	{
		Average       = b2_mixAverage,
		GeometricMean = b2_mixGeometricMean,
		Multiply      = b2_mixMultiply,
		Minimum       = b2_mixMinimum,
		Maximum       = b2_mixMaximum
	};

	struct RigidBody2D;
	struct Transform;

	struct Physic2DProp
	{
		inline static constexpr float GRAVITY = 9.80665f;

		// 중력 벡터, 기본적으로 y축 방향으로 -GRAVITY (아래 방향) 설정
		// 중력의 방향은 상황에 따라 변경 가능
		Vec2 gravity = { 0.f, -GRAVITY };

		// 물체 겹침시 탈출 속도
		float contactPushSpeed = 3.0f * b2GetLengthUnitsPerMeter();

		// 반발 계수 적용 임계값 (m/s)
		// 이 값 이상의 충돌 속도에서만 반발 계수가 적용됨
		float restitutionThreshold = b2GetLengthUnitsPerMeter();

		// 최대 선형 속도 (m/s)
		// 물체가 가질 수 있는 최대 속도, 시뮬레이션 안정성을 위해 제한
		float maximumLinearSpeed = 400.0f * b2GetLengthUnitsPerMeter();

		// 휴면 상태 활성화 여부
		// 움직이지 않는 물체를 휴면 상태로 전환해 성능 최적화
		bool enableSleep = true;

		// 연속 충돌 감지 활성화 여부
		// 빠르게 움직이는 물체 간 충돌을 정확히 계산하기 위해 사용
		bool enableContinuous = true;

		// 접촉 강성 (Hz)
		// 접촉 시의 상호 작용 강도를 결정 (값이 클수록 강성 증가)
		float contactHertz = 30.f;

		// 관절 강성 (Hz)
		// 관절의 연결 강도를 결정 (값이 클수록 강성 증가)
		float jointHertz = 60.f;

		// 접촉 감쇠 비율
		// 접촉 시 진동 에너지의 감쇠 정도를 나타냄
		float contactDampingRatio = 10.f;

		// 관절 감쇠 비율
		// 관절 연결 시 진동 에너지의 감쇠 정도를 나타냄
		float jointDampingRatio = 2.f;

		// 물리 계산에 사용할 스레드 수
		// CPU 모델의 성능 코어 수를 기준으로 설정
		int32 workerCount = 4;

		// 마찰력 혼합 규칙
		// 물체 간 마찰력을 계산할 때 사용하는 방식 (기본 값은 기하 평균)
		ePhysicMixRule frictionMixingRule = ePhysicMixRule::GeometricMean;

		// 반발 계수 혼합 규칙
		// 물체 간 반발력을 계산할 때 사용하는 방식 (기본 값은 최대값)
		ePhysicMixRule restitutionMixingRule = ePhysicMixRule::Maximum;

		// 물리 시뮬레이션의 세부 단계 수
		// 값이 클수록 정확도는 증가하지만 비용도 높아짐
		static Physic2DProp s_defaultProp;

		uint32	subStep = 4;
		bool	enableDebugDraw = false;
		Vec4	debugColor = Color::BLUE;
	};


	class Physic2D : public IPlugin
	{
	public:
		explicit			 Physic2D(const Physic2DProp& in_prop = Physic2DProp::s_defaultProp);
							~Physic2D() override;

		void				Init() override {}
		void				Shutdown() override;

		void				OnEnterScene() override;
		void				OnExitScene() override;

		void				OnUpdateScene(TimeStamp& in_ts) override;
		void				OnRenderScene(TimeStamp& in_ts) override;

		TypeID				GetTypeID() override { return TypeID::Generate<Physic2D>(); }
		void				SetPhysicProp(const Physic2DProp& in_prop) { m_prop = in_prop; }

		const Physic2DProp& GetPhysicProp() const { return m_prop; }
		b2WorldId			GetWorldID() const { return m_worldID; }

		void				DestroyWorld() const;

	private:
		b2WorldId		m_worldID = {};
		Physic2DProp	m_prop = {};
	};
}

