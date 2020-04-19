#pragma once
/****************************************************************************
## Image ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.03.20
*****************************************************************************/
class Image final
{
public:
	struct TagLoadedImageInfo
	{
		string key;
		wstring directory;

		TagLoadedImageInfo() {}
		TagLoadedImageInfo(const string& key, const wstring& directory)
			:key(key), directory(directory) {}
	};
	struct FrameRect
	{
		float x;
		float y;
		float width;
		float height;
	};
private:
	ID2D1Bitmap*		mBitmap;
	float				mAlpha;

	POINTFLOAT	mSize;
	float				mScale;
	float				mAngle;

	TagLoadedImageInfo	mLoadInfo;

	vector<Image::FrameRect>	mFrameInfo;
	int							mMaxFrameX;
	int							mMaxFrameY;

	bool						mIsReverseAxisX;
	bool						mIsReverseAxisY;
private:
	friend class ImageManager;
	Image() = delete;
	Image(ID2D1Bitmap*const bitmap, const TagLoadedImageInfo& loadinfo);
	Image(ID2D1Bitmap* bitmap, const TagLoadedImageInfo& loadinfo, const int& maxFrameX, const int& maxFrameY);
	virtual ~Image();
	Image operator = (const Image& image) {}
public:
	void Render(float x, float y,  Pivot::Enum pivot = Pivot::CENTER);
	void StretchRender(float x, float y, float scale, Pivot::Enum pivot);
	void StretchRotationRender(float x, float y, float _scale, float angle, Pivot::Enum pivot);
	void rotationRender(float x, float y, float angle, Pivot::Enum pivot);
	void AlphaRender(float x, float y, float alpha, Pivot::Enum pivot);
	void FrameRender(float x, float y,  int frameX,  int frameY,  Pivot::Enum pivot = Pivot::CENTER, bool loop = false);

	void AlphaFrameRender(float x, float y, int frameX, int frameY, float alpha, Pivot::Enum pivot);

	void ResetRenderOption();

	void SetSize(const POINTFLOAT& vec) { this->mSize = vec; }
	void SetAlpha(const float& alpha) { this->mAlpha = alpha; }
	void SetScale(const float& scale) { this->mScale = scale; }
	void SetAngle(const float& angle) { this->mAngle = angle; }
	void SetReverseX(const bool& x) { this->mIsReverseAxisX = x; }
	void SetReverseY(const bool& y) { this->mIsReverseAxisY = y; }

	 int GetMaxFrameX()const { return mMaxFrameX; }
	 int GetMaxFrameY()const { return mMaxFrameY; }
	 string GetKey() const { return this->mLoadInfo.key; }
	 UINT GetWidth() const { return mBitmap->GetPixelSize().width; }
	 UINT GetHeight()const { return mBitmap->GetPixelSize().height; }
	 POINTFLOAT GetSize()const { return this->mSize; }
	 POINTFLOAT GetFrameSize(const int& frame = 0) const { return { mFrameInfo[frame].width, mFrameInfo[frame].height }; }
	 TagLoadedImageInfo GetLoadInfo() const { return mLoadInfo; }
private:
	const POINTFLOAT GetPivotPosition( float x,  float y, Pivot::Enum pivot);
};

