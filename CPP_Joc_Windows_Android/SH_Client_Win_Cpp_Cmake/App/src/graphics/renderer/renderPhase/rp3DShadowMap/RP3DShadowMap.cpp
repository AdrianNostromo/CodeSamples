#include "RP3DShadowMap.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/screen/IScreen.h>
#include <graphics/shader/Shader.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/visual/Texture.h>
#include <graphics/util/GraphicsUtil.h>
#include <base/app/config/AppConfig.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include "../rp3DFinalOutput/IRenderPhase3DFinalOutput.h"
#include <graphics/geometry/data/FrameBufferObject.h>

using namespace graphics;

unsigned int RP3DShadowMap::SHADOW_WIDTH = 1024;
unsigned int RP3DShadowMap::SHADOW_HEIGHT = 1024;

bool RP3DShadowMap::IS_DEBUG_SHADOW_MAP = false;

RP3DShadowMap::RP3DShadowMap(
	int renderOrder,
	base::IScreen* screen)
	: super(
		renderOrder,
		screen)
{
	rawSubclassPointer = (IRP3DShadowMap*)this;

	viewportSizeOverride.set(SHADOW_WIDTH, SHADOW_HEIGHT);
}

void RP3DShadowMap::onIsEnabledChanged() {
	super::onIsEnabledChanged();

	if (!getIsEnabled()) {
		// Release the framebuffer and the texture.
		//asd_r;
	} else {
		{
			// Create the texture used for the fbo.
			Texture::TextureWrap const* wrapS = nullptr;
			Texture::TextureWrap const* wrapT = nullptr;
			Color* borderColorManaged = nullptr;

			if (IGL::CLAMP_TO_BORDER != 0 && IGL::TEXTURE_BORDER_COLOR != 0) {
				wrapS = &Texture::TextureWrap::ClampToBorder;
				wrapT = &Texture::TextureWrap::ClampToBorder;
				borderColorManaged = newt Color(1.0f, 1.0f, 1.0f, 1.0f);
			} else {
				// Feature not supported. On android, this must be done in the shader. Currently don't specify this parameter. Maybe in the future just ignore it but not now.
				wrapS = &Texture::TextureWrap::ClampToEdge;
				wrapT = &Texture::TextureWrap::ClampToEdge;
				borderColorManaged = nullptr;
			}

			depthMapTexture = newt Texture(
				nullptr/*localBuffer*/, false,
				SHADOW_WIDTH/*width*/, SHADOW_HEIGHT/*height*/, 1/*channelsCount*/, 1/*channels*/ * 32/*bits_per_pixel*/,
				1/*mipMapLevels*/, IGL::DEPTH_COMPONENT32F/*sizedInternalFormat*/,
				&Texture::TextureFilterMin::Nearest/*minFilter*/, &Texture::TextureFilterMag::Nearest/*magFilter*/,
				wrapS/*wrapS*/, wrapT/*wrapT*/,
				nullptr/*textureArray*/,
				borderColorManaged/*borderColorManaged*/, ITextureUnit::TextureBindSlots::shadowDepthMap3D/*autoBindSlot*/
			);
			depthMapTexture->reservedCreate();
		}

		// Create the shadow map framebuffer.
		fboDepthMap = new graphics::FrameBufferObject(IGL::NONE/*drawBuffer*/, IGL::NONE/*readBuffer*/, depthMapTexture/*depthMapTexture*/, ITextureUnit::TextureBindSlots::shadowDepthMap3D/*depthMapTextureAutoBindSlot*/);
		fboDepthMap->reservedCreate();

		// Set default framebuffer.
		fboDepthMap->unbind();
	}
}

void RP3DShadowMap::render(graphics::IEnvironment* environment) {
	// The texture doesn't need to be bound.

	fboDepthMap->bind();
	CallGLChecked(GLUtil::gl->clear(IGL::DEPTH_BUFFER_BIT));

	GLUtil::renderContext->setCullFace(
		true/*isCullFaceEnabled*/,
		IGL::BACK/*cullFaceMode*/
	);

	super::render(environment);

	// Set default framebuffer and make preparations.
	fboDepthMap->unbind();

	//asd_x;// add a settings option to toggle this.
	if (AppConfig::IS_DEBUG && IS_DEBUG_SHADOW_MAP) {
		debugRenderShadowMap();
	}
}

Texture* RP3DShadowMap::getShadowDepthMapTexture() {
	return depthMapTexture;
}

//asd_r;// add this function to GraphicsUtil to easily show a texture. Move variants allowed.
Shader* testPreviewShader = nullptr;
// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		GLUtil::gl->genVertexArrays(1, &quadVAO);
		GLUtil::gl->genBuffers(1, &quadVBO);

		GLUtil::gl->bindVertexArray(quadVAO);
		GLUtil::gl->bindBuffer(IGL::ARRAY_BUFFER, quadVBO);

		GLUtil::gl->bufferData(IGL::ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, IGL::STATIC_DRAW);

		GLUtil::gl->enableVertexAttribArray(0);
		GLUtil::gl->vertexAttribPointer(0, 3, IGL::FLOAT, IGL::FALSE, 5 * sizeof(float), (void*)0);
		GLUtil::gl->enableVertexAttribArray(1);
		GLUtil::gl->vertexAttribPointer(1, 2, IGL::FLOAT, IGL::FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	GLUtil::gl->bindVertexArray(quadVAO);
	GLUtil::gl->bindBuffer(IGL::ARRAY_BUFFER, quadVBO);
	testPreviewShader->begin();
	// Use the same tex bind slot to avoid extra tex binds.
	static std::string u_depthMap = "u_depthMap";

	testPreviewShader->getProgram()->setUniform1iOptional(u_depthMap, ITextureUnit::TextureBindSlots::shadowDepthMap3D);

	GLUtil::gl->drawArrays(IGL::TRIANGLE_STRIP, 0, 4);
	
	testPreviewShader->end();
	GLUtil::gl->bindVertexArray(0);
}

void RP3DShadowMap::debugRenderShadowMap() {
	// Bind the texture.
	depthMapTexture->bind(ITextureUnit::TextureBindSlots::shadowDepthMap3D);

	// Create a preview shader.
	if (testPreviewShader == nullptr) {
		testPreviewShader = GraphicsUtil::CreateFilesSourcesShader(
			"shaders/dev3DShadowDepthMapPreview.vert",
			"shaders/dev3DShadowDepthMapPreview.frag"
		);
	}

	// Render a quad in the bottom left as half axis size.
	CallGLChecked(GLUtil::gl->viewport(0, 0, screen->getWindowResolutionX() / 2, screen->getWindowResolutionY() / 2));

	renderQuad();
}

void RP3DShadowMap::prepareShaderUniforms(
	IShader* shader, graphics::IEnvironment* environment,
	IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms)
{
	super::prepareShaderUniforms(shader, environment, geometryProvider_Vertices, geometryProvider_Indices, geometryProvider_ModelTransforms);

	//void
}

void RP3DShadowMap::dispose() {
	// Release the framebuffer and the texture.
	//asd_r;

	super::dispose();
}

RP3DShadowMap::~RP3DShadowMap() {
	//void
}
