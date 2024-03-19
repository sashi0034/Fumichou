#include "stdafx.h"
#include "ApuStream.h"

#include "AudioForward.h"

namespace Nes
{
	ApuStream::ApuStream()
	{
		m_wave.resize(SampleRate_43653, s3d::WaveSample{0, 0});
	}

	void ApuStream::getAudio(float* left, float* right, size_t samplesToWrite)
	{
		const int buffer = BufferRemaining();
		const int writable = std::min(static_cast<int>(samplesToWrite), buffer);

		// バッファの内容をストリームする
		for (int i = 0; i < writable; ++i)
		{
			*left = m_wave[m_headIndex].left;
			*right = m_wave[m_headIndex].right;
			left++;
			right++;
			m_headIndex = (m_headIndex + 1) % m_wave.size();
		}

		// samplesToWrite の空いている部分はゼロでパディング
		for (int i = writable; i < samplesToWrite; ++i)
		{
			*left = 0;
			*right = 0;
			left++;
			right++;
		}
	}

	void ApuStream::PushSample(float left, float right)
	{
		m_wave[m_tailIndex].left = left;
		m_wave[m_tailIndex].right = right;

		m_tailIndex = (m_tailIndex + 1) % m_wave.size();
	}

	int ApuStream::BufferRemaining() const
	{
		return m_headIndex <= m_tailIndex
			       // tail も head も折り返していないとき
			       ? m_tailIndex - m_headIndex
			       // tail だけ折り返している状態のとき
			       : m_tailIndex + (m_wave.size() - m_headIndex);
	}

	int ApuStream::BufferSize() const
	{
		return m_wave.size();
	}
}
