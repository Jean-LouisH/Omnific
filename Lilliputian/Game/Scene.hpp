#pragma once

#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Queue.hpp"
#include "../Utilities/Collections/Map.hpp"
#include "Audio.hpp"
#include "Font.hpp"
#include "Texture.hpp"
#include "Subscene2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		uint64_t entityIDCount;
		uint64_t audioListenerIDCount;
		uint64_t audioSourceIDCount;
		uint64_t boxColliderIDCount;
		uint64_t cameraIDCount;
		uint64_t propertyAnimationIDCount;
		uint64_t rigidBodyIDCount;
		uint64_t spriteIDCount;
		Vector<Stack<Subscene2D>> subscene2Ds;

		Map<String, Audio> audios;
		Map<String, Font> fonts;
		Map<String, Texture> textures;

		Audio loadAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Texture loadTexture(const char* filepath);
	public:
		Scene();
		void incrementSubscene2D();
		Entity2D newEntity2D();
		AudioListener2D attachAudioListener2D(EntityID entityID);
		AudioSource2D attachAudioSource2D(EntityID entityID);
		BoxCollider2D attachBoxCollider2D(EntityID entityID);
		Camera2D attachCamera2D(EntityID entityID);
		PropertyAnimation attachPropertyAnimation(EntityID entityID);
		RigidBody2D attachRigidBody2D(EntityID entityID);
		Sprite2D attachSprite2D(EntityID entityID);
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}