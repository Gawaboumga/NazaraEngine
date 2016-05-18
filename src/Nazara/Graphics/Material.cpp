// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#ifndef NAZARA_RENDERER_OPENGL
#define NAZARA_RENDERER_OPENGL // Mandatory to include the OpenGL headers
#endif

#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Core/ErrorFlags.hpp>
#include <Nazara/Renderer/OpenGL.hpp>
#include <Nazara/Renderer/Renderer.hpp>
#include <Nazara/Renderer/UberShaderPreprocessor.hpp>
#include <cstring>
#include <memory>
#include <Nazara/Graphics/Debug.hpp>

namespace Nz
{
	namespace
	{
		const UInt8 r_basicFragmentShader[] = {
			#include <Nazara/Graphics/Resources/Shaders/Basic/core.frag.h>
		};

		const UInt8 r_basicVertexShader[] = {
			#include <Nazara/Graphics/Resources/Shaders/Basic/core.vert.h>
		};

		const UInt8 r_phongLightingFragmentShader[] = {
			#include <Nazara/Graphics/Resources/Shaders/PhongLighting/core.frag.h>
		};

		const UInt8 r_phongLightingVertexShader[] = {
			#include <Nazara/Graphics/Resources/Shaders/PhongLighting/core.vert.h>
		};
	}

	/*!
	* \brief Checks whether the parameters for the material are correct
	* \return true If parameters are valid
	*/

	bool MaterialParams::IsValid() const
	{
		if (!UberShaderLibrary::Has(shaderName))
			return false;

		return true;
	}

	/*!
	* \brief Applies shader to the material
	* \return Constant pointer to the shader
	*
	* \param shaderFlags Flags for the shader
	* \param textureUnit Unit for the texture GL_TEXTURE"i"
	* \param lastUsedUnit Optional argument to get the last texture unit
	*/

	const Shader* Material::Apply(UInt32 shaderFlags, UInt8 textureUnit, UInt8* lastUsedUnit) const
	{
		const ShaderInstance& instance = m_shaders[shaderFlags];
		if (!instance.uberInstance)
			GenerateShader(shaderFlags);

		instance.uberInstance->Activate();

		if (instance.uniforms[MaterialUniform_AlphaThreshold] != -1)
			instance.shader->SendFloat(instance.uniforms[MaterialUniform_AlphaThreshold], m_alphaThreshold);

		if (instance.uniforms[MaterialUniform_Ambient] != -1)
			instance.shader->SendColor(instance.uniforms[MaterialUniform_Ambient], m_ambientColor);

		if (instance.uniforms[MaterialUniform_Diffuse] != -1)
			instance.shader->SendColor(instance.uniforms[MaterialUniform_Diffuse], m_diffuseColor);

		if (instance.uniforms[MaterialUniform_Shininess] != -1)
			instance.shader->SendFloat(instance.uniforms[MaterialUniform_Shininess], m_shininess);

		if (instance.uniforms[MaterialUniform_Specular] != -1)
			instance.shader->SendColor(instance.uniforms[MaterialUniform_Specular], m_specularColor);

		if (m_alphaMap && instance.uniforms[MaterialUniform_AlphaMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_alphaMap);
			Renderer::SetTextureSampler(textureUnit, m_diffuseSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_AlphaMap], textureUnit);
			textureUnit++;
		}

		if (m_diffuseMap && instance.uniforms[MaterialUniform_DiffuseMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_diffuseMap);
			Renderer::SetTextureSampler(textureUnit, m_diffuseSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_DiffuseMap], textureUnit);
			textureUnit++;
		}

		if (m_emissiveMap && instance.uniforms[MaterialUniform_EmissiveMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_emissiveMap);
			Renderer::SetTextureSampler(textureUnit, m_diffuseSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_EmissiveMap], textureUnit);
			textureUnit++;
		}

		if (m_heightMap && instance.uniforms[MaterialUniform_HeightMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_heightMap);
			Renderer::SetTextureSampler(textureUnit, m_diffuseSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_HeightMap], textureUnit);
			textureUnit++;
		}

		if (m_normalMap && instance.uniforms[MaterialUniform_NormalMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_normalMap);
			Renderer::SetTextureSampler(textureUnit, m_diffuseSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_NormalMap], textureUnit);
			textureUnit++;
		}

		if (m_specularMap && instance.uniforms[MaterialUniform_SpecularMap] != -1)
		{
			Renderer::SetTexture(textureUnit, m_specularMap);
			Renderer::SetTextureSampler(textureUnit, m_specularSampler);
			instance.shader->SendInteger(instance.uniforms[MaterialUniform_SpecularMap], textureUnit);
			textureUnit++;
		}

		Renderer::SetRenderStates(m_states);

		if (lastUsedUnit)
			*lastUsedUnit = textureUnit;

		return instance.shader;
	}

	/*!
	* \brief Builds the material from parameters
	*
	* \param matData Data information for the material
	* \param matParams Parameters for the material
	*/

	void Material::BuildFromParameters(const ParameterList& matData, const MaterialParams& matParams)
	{
		Color color;
		bool isEnabled;
		float fValue;
		int iValue;
		String path;

		ErrorFlags errFlags(ErrorFlag_Silent | ErrorFlag_ThrowExceptionDisabled, true);


		if (matData.GetFloatParameter(MaterialData::AlphaThreshold, &fValue))
			SetAlphaThreshold(fValue);

		if (matData.GetBooleanParameter(MaterialData::AlphaTest, &isEnabled))
			EnableAlphaTest(isEnabled);

		if (matData.GetColorParameter(MaterialData::AmbientColor, &color))
			SetAmbientColor(color);

		if (matData.GetIntegerParameter(MaterialData::DepthFunc, &iValue))
			SetDepthFunc(static_cast<RendererComparison>(iValue));

		if (matData.GetBooleanParameter(MaterialData::DepthSorting, &isEnabled))
			EnableDepthSorting(isEnabled);

		if (matData.GetColorParameter(MaterialData::DiffuseColor, &color))
			SetDiffuseColor(color);

		if (matData.GetIntegerParameter(MaterialData::DstBlend, &iValue))
			SetDstBlend(static_cast<BlendFunc>(iValue));

		if (matData.GetIntegerParameter(MaterialData::FaceCulling, &iValue))
			SetFaceCulling(static_cast<FaceSide>(iValue));

		if (matData.GetIntegerParameter(MaterialData::FaceFilling, &iValue))
			SetFaceFilling(static_cast<FaceFilling>(iValue));

		if (matData.GetBooleanParameter(MaterialData::Lighting, &isEnabled))
			EnableLighting(isEnabled);

		if (matData.GetFloatParameter(MaterialData::LineWidth, &fValue))
			m_states.lineWidth = fValue;

		if (matData.GetFloatParameter(MaterialData::PointSize, &fValue))
			m_states.pointSize = fValue;

		if (matData.GetColorParameter(MaterialData::SpecularColor, &color))
			SetSpecularColor(color);

		if (matData.GetFloatParameter(MaterialData::Shininess, &fValue))
			SetShininess(fValue);

		if (matData.GetIntegerParameter(MaterialData::SrcBlend, &iValue))
			SetSrcBlend(static_cast<BlendFunc>(iValue));

		if (matData.GetBooleanParameter(MaterialData::Transform, &isEnabled))
			EnableTransform(isEnabled);

		// RendererParameter
		if (matData.GetBooleanParameter(MaterialData::Blending, &isEnabled))
			Enable(RendererParameter_Blend, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::ColorWrite, &isEnabled))
			Enable(RendererParameter_ColorWrite, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::DepthBuffer, &isEnabled))
			Enable(RendererParameter_DepthBuffer, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::DepthWrite, &isEnabled))
			Enable(RendererParameter_DepthWrite, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::FaceCulling, &isEnabled))
			Enable(RendererParameter_FaceCulling, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::ScissorTest, &isEnabled))
			Enable(RendererParameter_ScissorTest, isEnabled);

		if (matData.GetBooleanParameter(MaterialData::StencilTest, &isEnabled))
			Enable(RendererParameter_StencilTest, isEnabled);

		// Samplers
		if (matData.GetIntegerParameter(MaterialData::DiffuseAnisotropyLevel, &iValue))
			m_diffuseSampler.SetAnisotropyLevel(static_cast<UInt8>(iValue));

		if (matData.GetIntegerParameter(MaterialData::DiffuseFilter, &iValue))
			m_diffuseSampler.SetFilterMode(static_cast<SamplerFilter>(iValue));

		if (matData.GetIntegerParameter(MaterialData::DiffuseWrap, &iValue))
			m_diffuseSampler.SetWrapMode(static_cast<SamplerWrap>(iValue));

		if (matData.GetIntegerParameter(MaterialData::SpecularAnisotropyLevel, &iValue))
			m_specularSampler.SetAnisotropyLevel(static_cast<UInt8>(iValue));

		if (matData.GetIntegerParameter(MaterialData::SpecularFilter, &iValue))
			m_specularSampler.SetFilterMode(static_cast<SamplerFilter>(iValue));

		if (matData.GetIntegerParameter(MaterialData::SpecularWrap, &iValue))
			m_specularSampler.SetWrapMode(static_cast<SamplerWrap>(iValue));

		// Stencil
		if (matData.GetIntegerParameter(MaterialData::StencilCompare, &iValue))
			m_states.frontFace.stencilCompare = static_cast<RendererComparison>(iValue);

		if (matData.GetIntegerParameter(MaterialData::StencilFail, &iValue))
			m_states.frontFace.stencilFail = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::StencilPass, &iValue))
			m_states.frontFace.stencilPass = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::StencilZFail, &iValue))
			m_states.frontFace.stencilZFail = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::StencilMask, &iValue))
			m_states.frontFace.stencilMask = static_cast<UInt32>(iValue);

		if (matData.GetIntegerParameter(MaterialData::StencilReference, &iValue))
			m_states.frontFace.stencilReference = static_cast<unsigned int>(iValue);

		// Stencil (back)
		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilCompare, &iValue))
			m_states.backFace.stencilCompare = static_cast<RendererComparison>(iValue);

		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilFail, &iValue))
			m_states.backFace.stencilFail = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilPass, &iValue))
			m_states.backFace.stencilPass = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilZFail, &iValue))
			m_states.backFace.stencilZFail = static_cast<StencilOperation>(iValue);

		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilMask, &iValue))
			m_states.backFace.stencilMask = static_cast<UInt32>(iValue);

		if (matData.GetIntegerParameter(MaterialData::BackFaceStencilReference, &iValue))
			m_states.backFace.stencilReference = static_cast<unsigned int>(iValue);

		// Textures
		if (matParams.loadAlphaMap && matData.GetStringParameter(MaterialData::AlphaTexturePath, &path))
			SetAlphaMap(path);

		if (matParams.loadDiffuseMap && matData.GetStringParameter(MaterialData::DiffuseTexturePath, &path))
			SetDiffuseMap(path);

		if (matParams.loadEmissiveMap && matData.GetStringParameter(MaterialData::EmissiveTexturePath, &path))
			SetEmissiveMap(path);

		if (matParams.loadHeightMap && matData.GetStringParameter(MaterialData::HeightTexturePath, &path))
			SetHeightMap(path);

		if (matParams.loadNormalMap && matData.GetStringParameter(MaterialData::NormalTexturePath, &path))
			SetNormalMap(path);

		if (matParams.loadSpecularMap && matData.GetStringParameter(MaterialData::SpecularTexturePath, &path))
			SetSpecularMap(path);

		SetShader(matParams.shaderName);
	}

<<<<<<< HEAD
	/*!
	* \brief Enables a renderer parameter
	*
	* \param renderParameter Parameter for the rendering
	* \param enable Should the parameter be enabled
	*
	* \remark Produces a NazaraError with NAZARA_DEBUG if enumeration is invalid
	*/

	void Material::Enable(RendererParameter renderParameter, bool enable)
	{
		#ifdef NAZARA_DEBUG
		if (renderParameter > RendererParameter_Max)
		{
			NazaraError("Renderer parameter out of enum");
			return;
		}
		#endif

		m_states.parameters[renderParameter] = enable;
	}

	/*!
	* \brief Enables the alpha test
	*
	* \param alphaTest Should the parameter be enabled
	*
	* \remark This invalidates the shaders
	*/

	void Material::EnableAlphaTest(bool alphaTest)
	{
		m_alphaTestEnabled = alphaTest;

		InvalidateShaders();
	}

	/*!
	* \brief Enables the depth sorting
	*
	* \param depthSorting Should the parameter be enabled
	*
	* \remark This invalidates the shaders
	*/

	void Material::EnableDepthSorting(bool depthSorting)
	{
		m_depthSortingEnabled = depthSorting;
	}

	/*!
	* \brief Enables the lighting
	*
	* \param lighting Should the parameter be enabled
	*
	* \remark This invalidates the shaders
	*/

	void Material::EnableLighting(bool lighting)
	{
		m_lightingEnabled = lighting;

		InvalidateShaders();
	}

	/*!
	* \brief Enables the transformation
	*
	* \param transform Should the parameter be enabled
	*
	* \remark This invalidates the shaders
	*/

	void Material::EnableTransform(bool transform)
	{
		m_transformEnabled = transform;

		InvalidateShaders();
	}

	/*!
	* \brief Gets the alpha map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetAlphaMap() const
	{
		return m_alphaMap;
	}

	/*!
	* \brief Gets the alpha threshold
	* \return The threshold value for the alpha
	*/

	float Material::GetAlphaThreshold() const
	{
		return m_alphaThreshold;
	}

	/*!
	* \brief Gets the ambient color
	* \return Ambient color
	*/

	Color Material::GetAmbientColor() const
	{
		return m_ambientColor;
	}

	/*!
	* \brief Gets the function to compare depth
	* \return Function comparing the depth of two materials
	*/

	RendererComparison Material::GetDepthFunc() const
	{
		return m_states.depthFunc;
	}

	/*!
	* \brief Gets the diffuse color
	* \return Diffuse color
	*/

	Color Material::GetDiffuseColor() const
	{
		return m_diffuseColor;
	}

	/*!
	* \brief Gets the diffuse sampler
	* \return Reference to the current texture sampler for the diffuse
	*/

	TextureSampler& Material::GetDiffuseSampler()
	{
		return m_diffuseSampler;
	}

	/*!
	* \brief Gets the diffuse sampler
	* \return Constant reference to the current texture sampler for the diffuse
	*/

	const TextureSampler& Material::GetDiffuseSampler() const
	{
		return m_diffuseSampler;
	}

	/*!
	* \brief Gets the diffuse map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetDiffuseMap() const
	{
		return m_diffuseMap;
	}

	/*!
	* \brief Gets the dst in blend
	* \return Function for dst blending
	*/

	BlendFunc Material::GetDstBlend() const
	{
		return m_states.dstBlend;
	}

	/*!
	* \brief Gets the emissive map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetEmissiveMap() const
	{
		return m_emissiveMap;
	}

	/*!
	* \brief Gets the face culling
	* \return Current face culling side
	*/

	FaceSide Material::GetFaceCulling() const
	{
		return m_states.faceCulling;
	}

	/*!
	* \brief Gets the face filling
	* \return Current face filling
	*/

	FaceFilling Material::GetFaceFilling() const
	{
		return m_states.faceFilling;
	}

	/*!
	* \brief Gets the height map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetHeightMap() const
	{
		return m_heightMap;
	}

	/*!
	* \brief Gets the normal map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetNormalMap() const
	{
		return m_normalMap;
	}

	/*!
	* \brief Gets the render states
	* \return Constant reference to the render states
	*/

	const RenderStates& Material::GetRenderStates() const
	{
		return m_states;
	}

	/*!
	* \brief Gets the shader of this material
	* \return Constant pointer to the ubershader used
	*/

	const UberShader* Material::GetShader() const
	{
		return m_uberShader;
	}

	/*!
	* \brief Gets the shader instance based on the flag
	* \return Constant pointer to the ubershader instance
	*
	* \param flags Flag of the shader
	*/

	const UberShaderInstance* Material::GetShaderInstance(UInt32 flags) const
	{
		const ShaderInstance& instance = m_shaders[flags];
		if (!instance.uberInstance)
			GenerateShader(flags);

		return instance.uberInstance;
	}

	/*!
	* \brief Gets the shininess
	* \return Current shininess
	*/

	float Material::GetShininess() const
	{
		return m_shininess;
	}

	/*!
	* \brief Gets the specular color
	* \return Specular color
	*/

	Color Material::GetSpecularColor() const
	{
		return m_specularColor;
	}

	/*!
	* \brief Gets the specular map
	* \return Pointer to the current texture
	*/

	Texture* Material::GetSpecularMap() const
	{
		return m_specularMap;
	}

	/*!
	* \brief Gets the specular sampler
	* \return Reference to the current texture sampler for the specular
	*/

	TextureSampler& Material::GetSpecularSampler()
	{
		return m_specularSampler;
	}

	/*!
	* \brief Gets the specular sampler
	* \return Constant reference to the current texture sampler for the specular
	*/

	const TextureSampler& Material::GetSpecularSampler() const
	{
		return m_specularSampler;
	}

	/*!
	* \brief Gets the src in blend
	* \return Function for src blending
	*/

	BlendFunc Material::GetSrcBlend() const
	{
		return m_states.srcBlend;
	}

	/*!
	* \brief Checks whether this material has an alpha map
	* \return true If it is the case
	*/

	bool Material::HasAlphaMap() const
	{
		return m_alphaMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has a diffuse map
	* \return true If it is the case
	*/

	bool Material::HasDiffuseMap() const
	{
		return m_diffuseMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has an emissive map
	* \return true If it is the case
	*/

	bool Material::HasEmissiveMap() const
	{
		return m_emissiveMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has a height map
	* \return true If it is the case
	*/

	bool Material::HasHeightMap() const
	{
		return m_heightMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has a normal map
	* \return true If it is the case
	*/

	bool Material::HasNormalMap() const
	{
		return m_normalMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has a specular map
	* \return true If it is the case
	*/

	bool Material::HasSpecularMap() const
	{
		return m_specularMap.IsValid();
	}

	/*!
	* \brief Checks whether this material has alpha test enabled
	* \return true If it is the case
	*/

	bool Material::IsAlphaTestEnabled() const
	{
		return m_alphaTestEnabled;
	}

	/*!
	* \brief Checks whether this material has depth sorting enabled
	* \return true If it is the case
	*/

	bool Material::IsDepthSortingEnabled() const
	{
		return m_depthSortingEnabled;
	}

	/*!
	* \brief Checks whether this material has the render parameter enabled
	* \return true If it is the case
	*
	* \param parameter Parameter for the rendering
	*
	* \remark Produces a NazaraError with NAZARA_DEBUG defined if enumeration is invalid
	*/

	bool Material::IsEnabled(RendererParameter parameter) const
	{
		#ifdef NAZARA_DEBUG
		if (parameter > RendererParameter_Max)
		{
			NazaraError("Renderer parameter out of enum");
			return false;
		}
		#endif

		return m_states.parameters[parameter];
	}

	/*!
	* \brief Checks whether this material has lightning enabled
	* \return true If it is the case
	*/

	bool Material::IsLightingEnabled() const
	{
		return m_lightingEnabled;
	}

	/*!
	* \brief Checks whether this material has transformation enabled
	* \return true If it is the case
	*/

	bool Material::IsTransformEnabled() const
	{
		return m_transformEnabled;
	}

	/*!
	* \brief Loads the material from file
	* \return true if loading is successful
	*
	* \param filePath Path to the file
	* \param params Parameters for the material
	*/

	bool Material::LoadFromFile(const String& filePath, const MaterialParams& params)
	{
		return MaterialLoader::LoadFromFile(this, filePath, params);
	}

	/*!
	* \brief Loads the material from memory
	* \return true if loading is successful
	*
	* \param data Raw memory
	* \param size Size of the memory
	* \param params Parameters for the material
	*/

	bool Material::LoadFromMemory(const void* data, std::size_t size, const MaterialParams& params)
	{
		return MaterialLoader::LoadFromMemory(this, data, size, params);
	}

	/*!
	* \brief Loads the material from stream
	* \return true if loading is successful
	*
	* \param stream Stream to the material
	* \param params Parameters for the material
	*/

	bool Material::LoadFromStream(Stream& stream, const MaterialParams& params)
	{
		return MaterialLoader::LoadFromStream(this, stream, params);
	}

	/*!
	* \brief Resets the material, cleans everything
	*/

=======
>>>>>>> 9c106e0c3c9bcd20400550083e5a9d3dcfe36e07
	void Material::Reset()
	{
		OnMaterialReset(this);

		m_alphaMap.Reset();
		m_depthMaterial.Reset();
		m_diffuseMap.Reset();
		m_emissiveMap.Reset();
		m_heightMap.Reset();
		m_normalMap.Reset();
		m_specularMap.Reset();
		m_uberShader.Reset();

		for (ShaderInstance& instance : m_shaders)
			instance.uberInstance = nullptr;

		m_alphaThreshold = 0.2f;
		m_alphaTestEnabled = false;
		m_ambientColor = Color(128, 128, 128);
		m_depthSortingEnabled = false;
		m_diffuseColor = Color::White;
		m_diffuseSampler = TextureSampler();
		m_lightingEnabled = true;
		m_shadowCastingEnabled = true;
		m_shadowReceiveEnabled = true;
		m_shininess = 50.f;
		m_specularColor = Color::White;
		m_specularSampler = TextureSampler();
		m_states = RenderStates();
		m_states.parameters[RendererParameter_DepthBuffer] = true;
		m_states.parameters[RendererParameter_FaceCulling] = true;
		m_transformEnabled = true;

		SetShader("Basic");
	}

<<<<<<< HEAD
	/*!
	* \brief Sets the alpha map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetAlphaMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetAlphaMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the alpha map with a reference to a texture
	* \return true If successful
	*
	* \param alphaMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetAlphaMap(TextureRef alphaMap)
	{
		m_alphaMap = std::move(alphaMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the alpha threshold
	*
	* \param alphaThreshold Threshold for the alpha
	*/

	void Material::SetAlphaThreshold(float alphaThreshold)
	{
		m_alphaThreshold = alphaThreshold;
	}

	/*!
	* \brief Sets the color for ambient
	*
	* \param ambient Color for ambient
	*/

	void Material::SetAmbientColor(const Color& ambient)
	{
		m_ambientColor = ambient;
	}

	/*!
	* \brief Sets the depth functor
	*
	* \param depthFunc
	*/

	void Material::SetDepthFunc(RendererComparison depthFunc)
	{
		m_states.depthFunc = depthFunc;
	}

	/*!
	* \brief Sets the color for diffuse
	*
	* \param diffuse Color for diffuse
	*/

	void Material::SetDiffuseColor(const Color& diffuse)
	{
		m_diffuseColor = diffuse;
	}

	/*!
	* \brief Sets the diffuse map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetDiffuseMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetDiffuseMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the diffuse map with a reference to a texture
	* \return true If successful
	*
	* \param diffuseMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetDiffuseMap(TextureRef diffuseMap)
	{
		m_diffuseMap = std::move(diffuseMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the diffuse sampler
	*
	* \param sampler Diffuse sample
	*/

	void Material::SetDiffuseSampler(const TextureSampler& sampler)
	{
		m_diffuseSampler = sampler;
	}

	/*!
	* \brief Sets the dst in blend
	*
	* \param func Function for dst blending
	*/

	void Material::SetDstBlend(BlendFunc func)
	{
		m_states.dstBlend = func;
	}

	/*!
	* \brief Sets the emissive map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetEmissiveMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetEmissiveMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the emissive map with a reference to a texture
	* \return true If successful
	*
	* \param emissiveMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetEmissiveMap(TextureRef emissiveMap)
	{
		m_emissiveMap = std::move(emissiveMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the face culling
	*
	* \param faceSide Face to cull
	*/

	void Material::SetFaceCulling(FaceSide faceSide)
	{
		m_states.faceCulling = faceSide;
	}

	/*!
	* \brief Sets the face filling
	*
	* \param filling Face to fill
	*/

	void Material::SetFaceFilling(FaceFilling filling)
	{
		m_states.faceFilling = filling;
	}

	/*!
	* \brief Sets the height map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetHeightMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetHeightMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the height map with a reference to a texture
	* \return true If successful
	*
	* \param heightMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetHeightMap(TextureRef heightMap)
	{
		m_heightMap = std::move(heightMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the normal map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetNormalMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetNormalMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the normal map with a reference to a texture
	* \return true If successful
	*
	* \param normalMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetNormalMap(TextureRef normalMap)
	{
		m_normalMap = std::move(normalMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the render states
	*
	* \param states States for the rendering
	*/

	void Material::SetRenderStates(const RenderStates& states)
	{
		m_states = states;
	}

	/*!
	* \brief Sets the shader with a constant reference to a ubershader
	*
	* \param uberShader Uber shader to apply
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetShader(UberShaderConstRef uberShader)
	{
		m_uberShader = std::move(uberShader);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the shader by name
	* \return true If successful
	*
	* \param uberShaderName Named shader
	*/

	bool Material::SetShader(const String& uberShaderName)
	{
		UberShaderConstRef uberShader = UberShaderLibrary::Get(uberShaderName);
		if (!uberShader)
			return false;

		SetShader(std::move(uberShader));
		return true;
	}

	/*!
	* \brief Sets the shininess of the material
	*
	* \param shininess Value of the shininess
	*/

	void Material::SetShininess(float shininess)
	{
		m_shininess = shininess;
	}

	/*!
	* \brief Sets the color for specular
	*
	* \param specular Color
	*/

	void Material::SetSpecularColor(const Color& specular)
	{
		m_specularColor = specular;
	}

	/*!
	* \brief Sets the specular map by name
	* \return true If successful
	*
	* \param textureName Named texture
	*/

	bool Material::SetSpecularMap(const String& textureName)
	{
		TextureRef texture = TextureLibrary::Query(textureName);
		if (!texture)
		{
			texture = TextureManager::Get(textureName);
			if (!texture)
				return false;
		}

		SetSpecularMap(std::move(texture));
		return true;
	}

	/*!
	* \brief Sets the specular map with a reference to a texture
	* \return true If successful
	*
	* \param specularMap Texture
	*
	* \remark This invalidates the shaders
	*/

	void Material::SetSpecularMap(TextureRef specularMap)
	{
		m_specularMap = std::move(specularMap);

		InvalidateShaders();
	}

	/*!
	* \brief Sets the specular sampler
	*
	* \param sampler Specular sample
	*/

	void Material::SetSpecularSampler(const TextureSampler& sampler)
	{
		m_specularSampler = sampler;
	}

	/*!
	* \brief Sets the src in blend
	*
	* \param func Function for src blending
	*/

	void Material::SetSrcBlend(BlendFunc func)
	{
		m_states.srcBlend = func;
	}

	/*!
	* \brief Sets the current material with the content of the other one
	* \return A reference to this
	*
	* \param material The other Material
	*/

	Material& Material::operator=(const Material& material)
	{
		Resource::operator=(material);

		Copy(material);
		return *this;
	}

	/*!
	* \brief Gets the default material
	* \return Reference to the default material
	*/

	MaterialRef Material::GetDefault()
	{
		return s_defaultMaterial;
	}

	/*!
	* \brief Copies the other material
	*
	* \param material Material to copy into this
	*/

	void Material::Copy(const Material& material)
	{
		// Copy of base states
		m_alphaTestEnabled    = material.m_alphaTestEnabled;
		m_alphaThreshold      = material.m_alphaThreshold;
		m_ambientColor        = material.m_ambientColor;
		m_depthSortingEnabled = material.m_depthSortingEnabled;
		m_diffuseColor        = material.m_diffuseColor;
		m_diffuseSampler      = material.m_diffuseSampler;
		m_lightingEnabled     = material.m_lightingEnabled;
		m_shininess           = material.m_shininess;
		m_specularColor       = material.m_specularColor;
		m_specularSampler     = material.m_specularSampler;
		m_states              = material.m_states;
		m_transformEnabled    = material.m_transformEnabled;

		// Copy of reference to the textures
		m_alphaMap    = material.m_alphaMap;
		m_diffuseMap  = material.m_diffuseMap;
		m_emissiveMap = material.m_emissiveMap;
		m_heightMap   = material.m_heightMap;
		m_normalMap   = material.m_normalMap;
		m_specularMap = material.m_specularMap;

		// Copy of the reference to the Über-Shader
=======
	void Material::Copy(const Material& material)
	{
		// Copie des états de base
		m_alphaTestEnabled     = material.m_alphaTestEnabled;
		m_alphaThreshold       = material.m_alphaThreshold;
		m_ambientColor         = material.m_ambientColor;
		m_depthSortingEnabled  = material.m_depthSortingEnabled;
		m_diffuseColor         = material.m_diffuseColor;
		m_diffuseSampler       = material.m_diffuseSampler;
		m_lightingEnabled      = material.m_lightingEnabled;
		m_shininess            = material.m_shininess;
		m_shadowCastingEnabled = material.m_shadowCastingEnabled;
		m_shadowReceiveEnabled = material.m_shadowReceiveEnabled;
		m_specularColor        = material.m_specularColor;
		m_specularSampler      = material.m_specularSampler;
		m_states               = material.m_states;
		m_transformEnabled     = material.m_transformEnabled;

		// Copie des références de texture
		m_alphaMap      = material.m_alphaMap;
		m_depthMaterial = material.m_depthMaterial;
		m_diffuseMap    = material.m_diffuseMap;
		m_emissiveMap   = material.m_emissiveMap;
		m_heightMap     = material.m_heightMap;
		m_normalMap     = material.m_normalMap;
		m_specularMap   = material.m_specularMap;
>>>>>>> 9c106e0c3c9bcd20400550083e5a9d3dcfe36e07
		m_uberShader = material.m_uberShader;

		// We copy the instances of the shader too
		std::memcpy(&m_shaders[0], &material.m_shaders[0], (ShaderFlags_Max + 1) * sizeof(ShaderInstance));
	}

	/*!
	* \brief Generates the shader based on flag
	*
	* \param flags Flag for the shaer
	*/

	void Material::GenerateShader(UInt32 flags) const
	{
		ParameterList list;
		list.SetParameter("ALPHA_MAPPING", m_alphaMap.IsValid());
		list.SetParameter("ALPHA_TEST", m_alphaTestEnabled);
		list.SetParameter("COMPUTE_TBNMATRIX", m_normalMap.IsValid() || m_heightMap.IsValid());
		list.SetParameter("DIFFUSE_MAPPING", m_diffuseMap.IsValid());
		list.SetParameter("EMISSIVE_MAPPING", m_emissiveMap.IsValid());
		list.SetParameter("LIGHTING", m_lightingEnabled);
		list.SetParameter("NORMAL_MAPPING", m_normalMap.IsValid());
		list.SetParameter("PARALLAX_MAPPING", m_heightMap.IsValid());
		list.SetParameter("SHADOW_MAPPING", m_shadowReceiveEnabled);
		list.SetParameter("SPECULAR_MAPPING", m_specularMap.IsValid());
		list.SetParameter("TEXTURE_MAPPING", m_alphaMap.IsValid() || m_diffuseMap.IsValid() || m_emissiveMap.IsValid() ||
											 m_normalMap.IsValid() || m_heightMap.IsValid() || m_specularMap.IsValid() ||
											 flags & ShaderFlags_TextureOverlay);
		list.SetParameter("TRANSFORM", m_transformEnabled);

		list.SetParameter("FLAG_BILLBOARD", static_cast<bool>((flags & ShaderFlags_Billboard) != 0));
		list.SetParameter("FLAG_DEFERRED", static_cast<bool>((flags & ShaderFlags_Deferred) != 0));
		list.SetParameter("FLAG_INSTANCING", static_cast<bool>((flags & ShaderFlags_Instancing) != 0));
		list.SetParameter("FLAG_TEXTUREOVERLAY", static_cast<bool>((flags & ShaderFlags_TextureOverlay) != 0));
		list.SetParameter("FLAG_VERTEXCOLOR", static_cast<bool>((flags & ShaderFlags_VertexColor) != 0));

		ShaderInstance& instance = m_shaders[flags];
		instance.uberInstance = m_uberShader->Get(list);
		instance.shader = instance.uberInstance->GetShader();

		#define CacheUniform(name) instance.uniforms[MaterialUniform_##name] = instance.shader->GetUniformLocation("Material" #name)

		CacheUniform(AlphaMap);
		CacheUniform(AlphaThreshold);
		CacheUniform(Ambient);
		CacheUniform(Diffuse);
		CacheUniform(DiffuseMap);
		CacheUniform(EmissiveMap);
		CacheUniform(HeightMap);
		CacheUniform(NormalMap);
		CacheUniform(Shininess);
		CacheUniform(Specular);
		CacheUniform(SpecularMap);

		#undef CacheUniform
	}

<<<<<<< HEAD
	/*!
	* \brief Invalidates the shaders
	*/

	void Material::InvalidateShaders()
	{
		for (ShaderInstance& instance : m_shaders)
			instance.uberInstance = nullptr;
	}

	/*!
	* \brief Initializes the material librairies
	* \return true If successful
	*
	* \remark Produces a NazaraError if the material library failed to be initialized
	*/

=======
>>>>>>> 9c106e0c3c9bcd20400550083e5a9d3dcfe36e07
	bool Material::Initialize()
	{
		if (!MaterialLibrary::Initialize())
		{
			NazaraError("Failed to initialise library");
			return false;
		}

		if (!MaterialManager::Initialize())
		{
			NazaraError("Failed to initialise manager");
			return false;
		}

		// Basic shader
		{
			UberShaderPreprocessorRef uberShader = UberShaderPreprocessor::New();

			String fragmentShader(reinterpret_cast<const char*>(r_basicFragmentShader), sizeof(r_basicFragmentShader));
			String vertexShader(reinterpret_cast<const char*>(r_basicVertexShader), sizeof(r_basicVertexShader));

			uberShader->SetShader(ShaderStageType_Fragment, fragmentShader, "FLAG_TEXTUREOVERLAY ALPHA_MAPPING ALPHA_TEST AUTO_TEXCOORDS DIFFUSE_MAPPING");
			uberShader->SetShader(ShaderStageType_Vertex, vertexShader, "FLAG_BILLBOARD FLAG_INSTANCING FLAG_VERTEXCOLOR TEXTURE_MAPPING TRANSFORM UNIFORM_VERTEX_DEPTH");

			UberShaderLibrary::Register("Basic", uberShader);
		}

		// PhongLighting shader
		{
			UberShaderPreprocessorRef uberShader = UberShaderPreprocessor::New();

			String fragmentShader(reinterpret_cast<const char*>(r_phongLightingFragmentShader), sizeof(r_phongLightingFragmentShader));
			String vertexShader(reinterpret_cast<const char*>(r_phongLightingVertexShader), sizeof(r_phongLightingVertexShader));

			uberShader->SetShader(ShaderStageType_Fragment, fragmentShader, "FLAG_DEFERRED FLAG_TEXTUREOVERLAY ALPHA_MAPPING ALPHA_TEST AUTO_TEXCOORDS DIFFUSE_MAPPING EMISSIVE_MAPPING LIGHTING NORMAL_MAPPING PARALLAX_MAPPING SHADOW_MAPPING SPECULAR_MAPPING");
			uberShader->SetShader(ShaderStageType_Vertex, vertexShader, "FLAG_BILLBOARD FLAG_DEFERRED FLAG_INSTANCING FLAG_VERTEXCOLOR COMPUTE_TBNMATRIX LIGHTING PARALLAX_MAPPING SHADOW_MAPPING TEXTURE_MAPPING TRANSFORM UNIFORM_VERTEX_DEPTH");

			UberShaderLibrary::Register("PhongLighting", uberShader);
		}

<<<<<<< HEAD
		// Once base shaders registers, we can create default materials
		s_defaultMaterial = Material::New();
=======
		// Once the base shaders are registered, we can now set some default materials
		s_defaultMaterial = New();
>>>>>>> 9c106e0c3c9bcd20400550083e5a9d3dcfe36e07
		s_defaultMaterial->Enable(RendererParameter_FaceCulling, false);
		s_defaultMaterial->SetFaceFilling(FaceFilling_Line);
		MaterialLibrary::Register("Default", s_defaultMaterial);

        MaterialRef mat;

        mat = New();
        mat->Enable(RendererParameter_DepthWrite, false);
        mat->Enable(RendererParameter_FaceCulling, false);
        mat->EnableLighting(false);
        MaterialLibrary::Register("Basic2D", std::move(mat));

        mat = New();
        mat->Enable(RendererParameter_Blend, true);
        mat->Enable(RendererParameter_DepthWrite, false);
        mat->Enable(RendererParameter_FaceCulling, false);
        mat->EnableLighting(false);
        mat->SetDstBlend(BlendFunc_InvSrcAlpha);
        mat->SetSrcBlend(BlendFunc_SrcAlpha);
        MaterialLibrary::Register("Translucent2D", std::move(mat));

		return true;
	}

	/*!
	* \brief Uninitializes the material librairies
	*/

	void Material::Uninitialize()
	{
		s_defaultMaterial.Reset();
		UberShaderLibrary::Unregister("PhongLighting");
		UberShaderLibrary::Unregister("Basic");
		MaterialManager::Uninitialize();
		MaterialLibrary::Uninitialize();
	}

	MaterialLibrary::LibraryMap Material::s_library;
	MaterialLoader::LoaderList Material::s_loaders;
	MaterialManager::ManagerMap Material::s_managerMap;
	MaterialManager::ManagerParams Material::s_managerParameters;
	MaterialRef Material::s_defaultMaterial = nullptr;
}
