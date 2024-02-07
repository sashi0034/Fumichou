#pragma once

class ImGuiSiv3DAddon : public s3d::IAddon
{
public:
	ImGuiSiv3DAddon();

	bool init() override;

	bool update() override;

	void draw() const override;

	~ImGuiSiv3DAddon() override;

private:
	struct Impl;
	std::shared_ptr<Impl> p_impl;
};

class ImS3dTexture
{
public:
	explicit ImS3dTexture(const s3d::Texture& texture);
	s3d::Texture& GetTexture() const;
	s3d::Optional<ImTextureID> GetId() const;

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl{};
};
