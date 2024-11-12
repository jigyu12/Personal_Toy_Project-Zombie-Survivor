#include "stdafx.h"
#include "Animator.h"

void Animator::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * std::fabs(speed);
	if (accumTime < frameDuration)
		return;

	currentFrame += speed > 0.f ? 1 : -1;
	accumTime = 0.f;

	if (currentFrame == checkFrame)
	{
		switch (currentClip->loopType)
		{
		case AnimationLoopTypes::Single:
			currentFrame = speed > 0.f ? totalFrame - 1 : 0;
			break;
		case AnimationLoopTypes::Loop:
			currentFrame = speed > 0.f ? 0 : totalFrame - 1;
			break;
		}
	}

	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(const std::string& clipId)
{
	Play(&ANI_CLIP_MGR.Get(clipId));
}

void Animator::Play(AnimationClip* clip)
{
	isPlaying = true;

	currentClip = clip;
	totalFrame = clip->frames.size();
	checkFrame = this->speed > 0.f ? totalFrame : -1;
	currentFrame = speed > 0.f ? 0 : totalFrame - 1;

	frameDuration = 1.f / clip->fps;
	accumTime = 0.f;
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Stop()
{
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	sprite->setTexture(TEXTURE_MGR.Get(frame.texId));
	sprite->setTextureRect(frame.texCoord);
}