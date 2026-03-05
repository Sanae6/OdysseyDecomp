#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include <common/aglShaderEnum.h>

namespace agl {
class ShaderLocation;
class DrawContext;
}  // namespace agl

namespace al {
agl::ShaderLocation* getUniformBlockLocationMdlEnvView();
agl::ShaderLocation* getUniformBlockLocationMdlMtx();
agl::ShaderLocation* getUniformBlockLocationShp();
agl::ShaderLocation* getUniformBlockLocationRenderSky();
agl::ShaderLocation* getUniformBlockLocationEchoBlock();
agl::ShaderLocation* getUniformBlockLocationHdrTranslate();
agl::ShaderLocation* getUniformBlockLocationLightEnv();
agl::ShaderLocation* getUniformBlockLocationShadowMap();
agl::ShaderLocation* getUniformBlockLocationOtherFirst();
agl::ShaderLocation* getUniformBlockLocationOtherSecond();
agl::ShaderLocation* getUniformBlockLocationOtherThird();
agl::ShaderLocation* getSamplerLocationUniform0();
agl::ShaderLocation* getSamplerLocationUniform1();
agl::ShaderLocation* getSamplerLocationUniform2();
agl::ShaderLocation* getSamplerLocationUniform3();
agl::ShaderLocation* getSamplerLocationLinearDepth();
agl::ShaderLocation* getSamplerLocationGBufferBaseColor();
agl::ShaderLocation* getSamplerLocationGBufferNormal();
agl::ShaderLocation* getSamplerLocationCubeMapRoughness();
agl::ShaderLocation* getSamplerLocationMirrorTex();
agl::ShaderLocation* getSamplerLocationIndirectTex();
agl::ShaderLocation* getSamplerLocationDirLitTex();
agl::ShaderLocation* getSamplerLocationMaterialLight();
agl::ShaderLocation* getSamplerLocationShadowBuffer();
agl::ShaderLocation* getSamplerLocationMaterialLightSphere();
agl::ShaderLocation* getSamplerLocationShadowMap();
agl::ShaderLocation* getSamplerLocationCubeMapRoughnessRefract();
agl::ShaderLocation* getSamplerLocationProgTexture0();
agl::ShaderLocation* getSamplerLocationExposure();
agl::ShaderLocation* getSamplerLocationProgTextureCubemapGem0();
agl::ShaderLocation* getSamplerLocationProgTextureProcedural2D();
agl::ShaderLocation* getSamplerLocationProgTextureProcedural3D();
agl::ShaderLocation* getSamplerLocationFootPrintNormal();
agl::ShaderLocation* getSamplerLocationFootPrintBaseColor();
agl::ShaderLocation* getSamplerLocationHackStart();
void tryChangeShaderMode(agl::DrawContext*, agl::ShaderMode);
void forceChangeShaderMode(agl::DrawContext*, agl::ShaderMode);
void createMakeHalfTextureUbo();
void calcTanFovyHalf(sead::Vector2f*, sead::Vector2f*, f32, f32, const sead::Vector2f&);
}  // namespace al
