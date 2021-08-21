#include "tpch.h"
#include "renderer/renderall.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "util/utilall.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace tengine
{
	// opengl graphic context

	openglContext::openglContext(GLFWwindow* w)
		: win(w) {}

	void openglContext::init()
	{
		glfwMakeContextCurrent(win);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void openglContext::swapBuffers()
	{
		glfwSwapBuffers(win);
	}

	// buffer
	vBuffer* vBuffer::create(float* v, uint32_t s)
	{
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return new openglVBuffer(v, s);
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
	}
	vBuffer* vBuffer::create(uint32_t s)
	{
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return new openglVBuffer(s);
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
	}

	iBuffer* iBuffer::create(unsigned int* v, uint32_t s)
	{
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return new openglIBuffer(v, s);
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
	}

	frameBuffer* frameBuffer::create(const frameBufferProp& p)
	{
	
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return new openglFrameBuffer(p);
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
	}

	// opengl buffers
	openglVBuffer::openglVBuffer(float* v, uint32_t s)
	{
		glCreateBuffers(1, &renID);
		glBindBuffer(GL_ARRAY_BUFFER, renID);
		glBufferData(GL_ARRAY_BUFFER, s, v, GL_STATIC_DRAW);
	}
	openglVBuffer::openglVBuffer(uint32_t s)
	{
		glCreateBuffers(1, &renID);
		glBindBuffer(GL_ARRAY_BUFFER, renID);
		glBufferData(GL_ARRAY_BUFFER, s, 0, GL_DYNAMIC_DRAW);
	}

	openglVBuffer::~openglVBuffer()
	{
		glDeleteBuffers(1, &renID);
	}
	void openglVBuffer::setData(const void* data, uint32_t size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void openglVBuffer::bind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renID);
	}
	void openglVBuffer::unbind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	openglIBuffer::openglIBuffer(unsigned int* i, uint32_t c)
		: count(c)
	{
		glCreateBuffers(1, &renID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), i, GL_STATIC_DRAW);
	}

	openglIBuffer::~openglIBuffer()
	{
		glDeleteBuffers(1, &renID);
	}

	void openglIBuffer::bind()const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renID);
	}
	void openglIBuffer::unbind()const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	openglFrameBuffer::openglFrameBuffer(const frameBufferProp& p)
	{
		log("frame buffer created", 0);
		prop = p;
		invalidate();
	}
	openglFrameBuffer::~openglFrameBuffer()
	{
		log("frame buffer deleted, you should not see this unless you have terminated the program", 2);
		//glDeleteFramebuffers(1, &renID);
		//glDeleteTextures(1, &depthAttachment);
		//glDeleteTextures(1, &colorAttachment);
	}
	void openglFrameBuffer::invalidate()
	{
		if (renID != 0)
		{
			glDeleteFramebuffers(1, &renID);
			glDeleteTextures(1, &depthAttachment);
			glDeleteTextures(1, &colorAttachment);
		}

		glCreateFramebuffers(1, &renID);
		glBindFramebuffer(GL_FRAMEBUFFER, renID);
		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, prop.width, prop.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, prop.width, prop.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
		t_assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, 3, "Frame buffer construction failed");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void openglFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renID);
	}
	void openglFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void openglFrameBuffer::resize(float x, float y)
	{
		if (prop.height == y &&
			prop.width == x) return;
		prop.height = (uint32_t)y;
		prop.width = (uint32_t)x;
		invalidate();
	}

	// renderer
	void renderer::onResized(uint32_t w, uint32_t h)
	{
		renderCommand::resizeWin(0, 0, w, h);
	}
	void renderer::init()
	{
		renderCommand::init();
	}
	void renderer::beginScene(const orthographicCamera& cam)
	{

	}
	void renderer::endScene()
	{

	}
	void renderer::submit(
		const std::shared_ptr<vArray>& va,
		const std::shared_ptr<shader>& s,
		const orthographicCamera& c,
		const glm::mat4& transform)
	{
		s->bind();
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl:
			s->setUnimat4("uViewProject", c.viewProject);
			s->setUnimat4("uTransform", transform);
			break;
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
		va->bind();
		renderCommand::drawIndex(va);
	}

	// renderer2D
	renderStorage renderer2D::storage2D = renderStorage();
	void renderer2D::beginScene(const camera& c, const glm::mat4& transform)
	{
		storage2D.shaLib["defaultShader"]->bind();
		storage2D.shaLib["defaultShader"]->setUnimat4("uViewProject", 
			c.projection * glm::inverse(transform));

		storage2D.qvPtr = storage2D.qvBase;
		storage2D.quadIndexCount = 0;
		storage2D.textureSlotIndex = 1;
	}
	void renderer2D::endScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)storage2D.qvPtr - (uint8_t*)storage2D.qvBase);
		storage2D.vb->setData(storage2D.qvBase, dataSize);

		storage2D.vb->bind();
		storage2D.va->bind();
		storage2D.va->getIB()->bind();
		flush();
	}
	void renderer2D::flush()
	{
		for (uint32_t i = 0; i < storage2D.textureSlotIndex; i++)
		{
			storage2D.textureSlot[i]->bind(i);
		}
		int samplers[storage2D.maxTex];
		for (int i = 0; i < storage2D.maxTex; i++) samplers[i] = i;
		storage2D.shaLib["defaultShader"]->setUniintArray("uTexture", samplers, storage2D.maxTex);
		renderCommand::drawIndex(storage2D.va, storage2D.quadIndexCount);
	}
	void renderer2D::init()
	{
		log("2D renderer initialization...", 0);
		std::shared_ptr<vBuffer> vb;
		storage2D.va.reset(vArray::create());
		storage2D.vb.reset(vBuffer::create(storage2D.maxVertice * sizeof(quadVertex)));
		bufferLayout bl =
		{
			{"laPos", shaderDatatype::float3},
			{"laCord", shaderDatatype::float2},
			{"laColor", shaderDatatype::float4},
			{"laTex", shaderDatatype::float1},
		};
		storage2D.vb->layout = bl;
		storage2D.va->addVB(storage2D.vb);
		storage2D.qvBase = new quadVertex[storage2D.maxVertice];

		uint32_t* quadIndices = new uint32_t[storage2D.maxIndex];
		std::shared_ptr<iBuffer> ib;
		int offset = 0;
		for (unsigned int i = 0; i < storage2D.maxIndex; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 3;
			quadIndices[i + 3] = offset + 3;
			quadIndices[i + 4] = offset + 2;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		ib.reset(iBuffer::create(quadIndices, storage2D.maxIndex));
		storage2D.va->addIB(ib);
		delete[] quadIndices;
		storage2D.shaLib.insert({ "defaultShader", std::shared_ptr<shader>(shader::create("asset/shader/shader2D.glsl")) });

		memset(storage2D.textureSlot.data(), 0, storage2D.textureSlot.size() * sizeof(uint32_t));
		storage2D.textureSlot[0] = texture2D::create(DEFAULT_WHITE_PIC);

	}
	void renderer2D::shutdown()
	{

	}

	void renderer2D::drawQuad(
		const glm::mat4& transform,
		const glm::vec4& color, const std::string& texPath,
		const glm::vec2& coord1, const glm::vec2& coord2,
		const glm::vec2& coord3, const glm::vec2& coord4)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage2D.textureSlotIndex; i++)
		{
			if (texPath == "") break;

			if (texPath == storage2D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && texPath != "" && texPath != DEFAULT_WHITE_PIC)
		{
			textureIndex = (float)storage2D.textureSlotIndex;
			storage2D.textureSlot[storage2D.textureSlotIndex] = texture2D::create(texPath.c_str());
			storage2D.textureSlotIndex++;
		}
		glm::vec4 p1 = { -0.5f, -0.5f, 0, 1 };
		glm::vec4 p2 = { 0.5f, -0.5f, 0, 1 };
		glm::vec4 p3 = { -0.5f, 0.5f, 0, 1 };
		glm::vec4 p4 = { 0.5f, 0.5f, 0, 1 };

		storage2D.qvPtr->pos = transform * p1;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord1;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p2;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord2;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p3;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord3;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p4;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord4;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;

		storage2D.quadIndexCount += 6;
	}
	void renderer2D::drawQuad(
		const glm::mat4& transform,
		const glm::vec4& color,
		const std::shared_ptr<texture2D>& tex,
		const glm::vec2& coord1, const glm::vec2& coord2,
		const glm::vec2& coord3, const glm::vec2& coord4)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage2D.textureSlotIndex; i++)
		{
			if (!tex) break;

			if (tex->src == storage2D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && tex->src != DEFAULT_WHITE_PIC)
		{
			textureIndex = (float)storage2D.textureSlotIndex;
			storage2D.textureSlot[storage2D.textureSlotIndex] = tex;
			storage2D.textureSlotIndex++;
		}
		glm::vec4 p1 = { -0.5f, -0.5f, 0, 1 };
		glm::vec4 p2 = { 0.5f, -0.5f, 0, 1 };
		glm::vec4 p3 = { -0.5f, 0.5f, 0, 1 };
		glm::vec4 p4 = { 0.5f, 0.5f, 0, 1 };

		storage2D.qvPtr->pos = transform * p1;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord1;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p2;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord2;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p3;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord3;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;
		storage2D.qvPtr->pos = transform * p4;
		storage2D.qvPtr->color = color;
		storage2D.qvPtr->cord = coord4;
		storage2D.qvPtr->texID = textureIndex;
		storage2D.qvPtr++;

		storage2D.quadIndexCount += 6;
	}

	// renderer3D
	renderStorage renderer3D::storage3D = renderStorage();
	void renderer3D::beginScene(const camera& c, const glm::mat4& transform)
	{
		storage3D.shaLib["defaultShader"]->bind();
		storage3D.shaLib["defaultShader"]->setUnimat4("u_MVP",
			c.projection * glm::inverse(transform));

		storage3D.qvPtr = storage3D.qvBase;
		storage3D.quadIndexCount = 0;
		storage3D.textureSlotIndex = 1;
	}
	void renderer3D::endScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)storage3D.qvPtr - (uint8_t*)storage3D.qvBase);
		storage3D.vb->setData(storage3D.qvBase, dataSize);

		storage3D.vb->bind();
		storage3D.va->bind();
		storage3D.va->getIB()->bind();
		flush();
	}
	void renderer3D::init()
	{
		log("3D renderer initialization...", 0);
		std::shared_ptr<vBuffer> vb;
		storage3D.va.reset(vArray::create());
		storage3D.vb.reset(vBuffer::create(storage3D.maxVertice * sizeof(quadVertex)));
		bufferLayout bl =
		{
			{"laPos", shaderDatatype::float3},
			{"laCord", shaderDatatype::float2},
			{"laColor", shaderDatatype::float4},
			{"laTex", shaderDatatype::float1},
		};
		storage3D.vb->layout = bl;
		storage3D.va->addVB(storage3D.vb);
		storage3D.qvBase = new quadVertex[storage3D.maxVertice];
		uint32_t* quadIndices = new uint32_t[storage3D.maxIndex];
		std::shared_ptr<iBuffer> ib;
		for (unsigned int i = 0; i < storage3D.maxIndex; i++)
			quadIndices[i] = i;
		ib.reset(iBuffer::create(quadIndices, storage3D.maxIndex));
		storage3D.va->addIB(ib);
		delete[] quadIndices;
		storage3D.shaLib.insert({ "defaultShader", std::shared_ptr<shader>(shader::create("asset/shader/shader3D.glsl")) });
		memset(storage3D.textureSlot.data(), 0, storage3D.textureSlot.size() * sizeof(uint32_t));
		storage3D.textureSlot[0] = texture2D::create(DEFAULT_WHITE_PIC);
	}
	void renderer3D::shutdown()
	{

	}
	void renderer3D::flush()
	{
		for (uint32_t i = 0; i < storage3D.textureSlotIndex; i++)
			storage3D.textureSlot[i]->bind(i);
		int samplers[storage3D.maxTex];
		for (int i = 0; i < storage3D.maxTex; i++) samplers[i] = i;
		storage3D.shaLib["defaultShader"]->setUniintArray("uTexture", samplers, storage3D.maxTex);
		renderCommand::drawIndex(storage3D.va, storage3D.quadIndexCount);
	}
	void renderer3D::setLight(const glm::mat4& lightTransform,
		const glm::vec3& lightColor, const glm::vec3& lightToward,
		float lightIntensity)
	{

	}
	void renderer3D::drawTri(const std::string& texPath, const glm::vec4& color,
		const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage3D.textureSlotIndex; i++)
		{
			if (texPath == "" || texPath == DEFAULT_WHITE_PIC) break;
			if (texPath == storage3D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && texPath != DEFAULT_WHITE_PIC && texPath!= "")
		{
			textureIndex = (float)storage3D.textureSlotIndex;
			storage3D.textureSlot[storage3D.textureSlotIndex] = texture2D::create(texPath.c_str());
			storage3D.textureSlotIndex++;
		}

		storage3D.qvPtr->pos = p1;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord1;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p2;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord2;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p3;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord3;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;

		storage3D.quadIndexCount += 3;
	}
	void renderer3D::drawTri(const std::shared_ptr<texture2D>& tex, const glm::vec4& color,
		const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage3D.textureSlotIndex; i++)
		{
			if (!tex) break;
			if (tex->src == storage3D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && tex->src != DEFAULT_WHITE_PIC)
		{
			textureIndex = (float)storage3D.textureSlotIndex;
			storage3D.textureSlot[storage3D.textureSlotIndex] = tex;
			storage3D.textureSlotIndex++;
		}

		storage3D.qvPtr->pos = p1;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord1;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p2;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord2;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p3;
		storage3D.qvPtr->color = color;
		storage3D.qvPtr->cord = coord3;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;

		storage3D.quadIndexCount += 3;
	}
	void renderer3D::drawTri(const std::string& texPath,
		const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3,
		const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage3D.textureSlotIndex; i++)
		{
			if (texPath == "" || texPath == DEFAULT_WHITE_PIC) break;
			if (texPath == storage3D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && texPath != DEFAULT_WHITE_PIC && texPath != "")
		{
			textureIndex = (float)storage3D.textureSlotIndex;
			storage3D.textureSlot[storage3D.textureSlotIndex] = texture2D::create(texPath.c_str());
			storage3D.textureSlotIndex++;
		}

		storage3D.qvPtr->pos = p1;
		storage3D.qvPtr->color = color1;
		storage3D.qvPtr->cord = coord1;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p2;
		storage3D.qvPtr->color = color2;
		storage3D.qvPtr->cord = coord2;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p3;
		storage3D.qvPtr->color = color3;
		storage3D.qvPtr->cord = coord3;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;

		storage3D.quadIndexCount += 3;
	}
	void renderer3D::drawTri(const std::shared_ptr<texture2D>& tex,
		const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3,
		const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3)
	{
		float textureIndex = 0;
		for (uint32_t i = 0; i < storage3D.textureSlotIndex; i++)
		{
			if (!tex) break;
			if (tex->src == storage3D.textureSlot[i]->src)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0 && tex->src != DEFAULT_WHITE_PIC)
		{
			textureIndex = (float)storage3D.textureSlotIndex;
			storage3D.textureSlot[storage3D.textureSlotIndex] = tex;
			storage3D.textureSlotIndex++;
		}

		storage3D.qvPtr->pos = p1;
		storage3D.qvPtr->color = color1;
		storage3D.qvPtr->cord = coord1;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p2;
		storage3D.qvPtr->color = color2;
		storage3D.qvPtr->cord = coord2;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;
		storage3D.qvPtr->pos = p3;
		storage3D.qvPtr->color = color3;
		storage3D.qvPtr->cord = coord3;
		storage3D.qvPtr->texID = textureIndex;
		storage3D.qvPtr++;

		storage3D.quadIndexCount += 3;
	}

	// vertex array
	vArray* vArray::create()
	{
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return new openglVarray();
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
	}

	//openglVarray

	GLenum getOpenglType(const bufferElement& be)
	{
		switch (be.type)
		{
		case shaderDatatype::float1: return GL_FLOAT;
		case shaderDatatype::float2: return GL_FLOAT;
		case shaderDatatype::float3: return GL_FLOAT;
		case shaderDatatype::float4: return GL_FLOAT;
		case shaderDatatype::int1: return GL_INT;
		case shaderDatatype::int2: return GL_INT;
		case shaderDatatype::int3: return GL_INT;
		case shaderDatatype::int4: return GL_INT;
		case shaderDatatype::mat3: return GL_FLOAT;
		case shaderDatatype::mat4: return GL_FLOAT;
		case shaderDatatype::boool: return GL_BOOL;
		}

		log("unknown shader datatype", 2);
		return 0;
	}

	openglVarray::openglVarray()
	{
		glCreateVertexArrays(1, &renID);
	}
	openglVarray::~openglVarray()
	{
		glDeleteVertexArrays(1, &renID);
	}
	void openglVarray::bind() const
	{
		glBindVertexArray(renID);
	}
	void openglVarray::unbind() const
	{
		glBindVertexArray(0);
	}
	void openglVarray::addVB(const std::shared_ptr<vBuffer> vb)
	{
		glBindVertexArray(renID);
		vb->bind();
		bufferLayout bl = vb->layout;
		uint32_t ind = 0;
		for (const auto& e : bl)
		{
			glEnableVertexAttribArray(ind);
			glVertexAttribPointer(ind, e.getCount(), getOpenglType(e), e.normalized ? GL_TRUE : GL_FALSE,
				bl.getStride(), (const void*)(uint64_t)e.offset);
			ind++;
		}
		vbList.push_back(vb);
	}
	void openglVarray::addIB(const std::shared_ptr<iBuffer> _ib)
	{
		glBindVertexArray(renID);
		_ib->bind();
		ib = _ib;
	}

	// renderAPI
	API renderAPI::api = API::opengl;

	// renderCommand 
	renderAPI* renderCommand::renAPI = new openglRenderAPI();

	// opengl render API
	void openglRenderAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}
	void openglRenderAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void openglRenderAPI::setClear(const glm::vec4& c)
	{
		glClearColor(c.r, c.g, c.b, c.a);
	}
	void openglRenderAPI::drawIndex(const std::shared_ptr<vArray>& va, uint32_t count)
	{
		//uint32_t c = count == 0 ? va->getIB()->getCount() : count
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void openglRenderAPI::onResized(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		glViewport(x, y, w, h);
	}

	// texture
	std::shared_ptr<texture2D> texture2D::create(const char* texPath)
	{
		switch (renderer::getAPI())
		{
		case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		case API::opengl: return std::shared_ptr<texture2D>(new openglTex2D(texPath));
		default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		}
		return NULL;
	}

	// opengl texture
	openglTex2D::openglTex2D(const char* texPath)
		: path(texPath)
	{
		src = texPath;
		int w, h, channel;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path, &w, &h, &channel, 0);
		if (!data) data = stbi_load(DEFAULT_WHITE_PIC, &w, &h, &channel, 0);
		width = w; height = h;

		glCreateTextures(GL_TEXTURE_2D, 1, &renID);
		glTextureParameteri(renID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (channel == 4)
		{
			iFormat = GL_RGBA32F;
			dFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			iFormat = GL_RGB32F;
			dFormat = GL_RGB;
		}
		else log("image format not supported", 2);
		glTextureStorage2D(renID, 1, iFormat, width, height);
		glTextureSubImage2D(renID, 0, 0, 0, width, height, dFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	openglTex2D::~openglTex2D()
	{
		glDeleteTextures(1, &renID);
	}
	uint32_t openglTex2D::getWidth() const
	{
		return width;
	}
	uint32_t openglTex2D::getHeight() const
	{
		return height;
	}
	void openglTex2D::setData(void* data, uint32_t size)
	{
		uint32_t tmp = dFormat == GL_RGBA ? 4 : 3;
		t_assert(size == width * height * tmp, 2, "setData: size given is wrong!");
		glTextureSubImage2D(renID, 0, 0, 0, width, height, dFormat, GL_UNSIGNED_BYTE, data);
	}
	void openglTex2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, renID);
	}
}