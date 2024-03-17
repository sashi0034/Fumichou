#pragma once

namespace Nes
{
	class ApuStream : public s3d::IAudioStream
	{
	public:
		ApuStream();
		~ApuStream() override = default;
		void getAudio(float* left, float* right, size_t samplesToWrite) override;
		bool hasEnded() override { return false; };
		void rewind() override { return; }

		void PushSample(float left, float right);

		int BufferRemaining() const;
		int BufferSize() const;

	private:
		s3d::Wave m_wave{};
		int m_headIndex{};
		int m_tailIndex{};
	};
}
