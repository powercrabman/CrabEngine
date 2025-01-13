#pragma once

namespace crab
{
	constexpr float SPRITE_SCALE_FACTOR = 1 / 1000.f;

	inline void OnRenderSpriteSystem(Scene* in_scene)
	{
		auto view = in_scene->GetView<Transform, SpriteRender>();
		for (auto entity : view)
		{
			auto [transform, spRender] = view.get<Transform, SpriteRender>(entity);

			const Sprite* sprite = TryGetAsset(spRender.spriteID);
			if (!sprite) continue;

			const GameTexture* tex = TryGetAsset(sprite->textureID);
			if (!tex) continue;

			const SpriteDescription& desc = sprite->desc;

			auto [texWidth, texHeight] = tex->texture->GetSize();
			const float normWidth = desc.width / texWidth;
			const float normHeight = desc.height / texHeight;
			const Vec2 uv0 = { normWidth * desc.pivotCol, normHeight * desc.pivotRow };
			const Vec2 uv1 = uv0 + Vec2{ normWidth , normHeight };

			Draw2D::AddSprite(
				transform.GetWorld(),
				Vec2{ static_cast<float>(desc.width), static_cast<float>(desc.height) } *SPRITE_SCALE_FACTOR,
				spRender.color,
				tex->texture,
				uv0, uv1,
				static_cast<uint32>(entity)
			);
		}
	}

	inline void OnRenderSphereSystem(Scene* in_scene)
	{
		auto view = in_scene->GetView<SphereRender, Transform>();

		for (auto entity : view)
		{
			auto [spRender, trans] = view.get<SphereRender, Transform>(entity);
			Draw2D::AddCircleFill(trans.GetWorld(), spRender.radius, spRender.color, static_cast<uint32>(entity));
		}
	}

	inline void OnRenderBoxSystem(Scene* in_scene)
	{
		auto view = in_scene->GetView<BoxRender, Transform>();

		for (auto entity : view)
		{
			auto [in_boxRender, in_trans] = view.get<BoxRender, Transform>(entity);
			Draw2D::AddRectFill(in_trans.GetWorld(), in_boxRender.size, in_boxRender.color, static_cast<uint32>(entity));
		}
	}

	inline void OnUpdateFlipbookSystem(Scene* in_scene, TimeStamp& in_ts)
	{
		// 애니메이션 진행 갱신
		in_scene->GetView<FlipbookRender>().each([&](FlipbookRender& in_fbRender)
			{
				const Flipbook* fb = TryGetAsset(in_fbRender.assetID);
				if (fb && !in_fbRender.isFinish && in_fbRender.isPlay)
				{
					const FlipbookDescription& desc = fb->desc;

					in_fbRender.timeAcc += in_ts.deltaTime;
					float duration = desc.duration * (1.f / in_fbRender.playbackSpeed);
					if (in_fbRender.timeAcc >= duration)
					{
						in_fbRender.timeAcc = std::fmodf(in_fbRender.timeAcc, duration);
						++in_fbRender.currentFrame;

						if (in_fbRender.currentFrame >= desc.totalFrame - 1)
						{
							if (desc.loop)
							{
								in_fbRender.currentFrame = 0;
							}
							else
							{
								in_fbRender.isFinish = true;
							}
						}
					}
				}
			}
		);
	}

	inline void OnRenderFlipbookSystem(Scene* in_scene)
	{
		auto view = in_scene->GetView<Transform, FlipbookRender>();

		for (auto entity : view)
		{
			auto [in_trans, in_fbRender] = view.get<Transform, FlipbookRender>(entity);

			const Flipbook* fb = TryGetAsset(in_fbRender.assetID);
			if (!fb) continue;

			const GameTexture* tex = TryGetAsset(fb->textureID);
			if (!tex) continue;

			const FlipbookDescription& desc = fb->desc;
			auto [texWidth, texHeight] = tex->texture->GetSize();
			const float normWidth = desc.width / texWidth;
			const float normHeight = desc.height / texHeight;
			const Vec2 uv0 = { normWidth * (desc.pivotCol + in_fbRender.currentFrame) , normHeight * desc.pivotRow };
			const Vec2 uv1 = uv0 + Vec2{ normWidth , normHeight };

			Draw2D::AddSprite(
				in_trans.GetWorld(),
				Vec2{ static_cast<float>(desc.width), static_cast<float>(desc.height) } *SPRITE_SCALE_FACTOR,
				in_fbRender.color,
				tex->texture,
				uv0, uv1,
				static_cast<uint32>(entity)
			);
		}
	}

}
