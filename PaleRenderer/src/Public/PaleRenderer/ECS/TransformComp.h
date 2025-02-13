#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
	struct PALE_API SCompTransform
	{
		glm::vec3 _Position = glm::vec3{ 0.0, 0.0, 0.0 };
		glm::vec3 _Rotation = glm::vec3{ 0.0, 0.0, 0.0 };
		glm::vec3 _Scale = glm::vec3{ 1.0, 1.0, 1.0 };

		glm::vec3 _InitPosition = glm::vec3{ 0.0, 0.0, 0.0 };
		glm::vec3 _InitRotation = glm::vec3{ 0.0, 0.0, 0.0 };
		glm::vec3 _InitScale = glm::vec3{ 1.0, 1.0, 1.0 };

		SCompTransform() = default;
		SCompTransform(const SCompTransform&) = default;

		SCompTransform(const glm::vec3& vPosition, const glm::vec3& vRotation, const glm::vec3& vScale)
			: _InitPosition(vPosition), _InitRotation(vRotation), _InitScale(vScale),
			_Position(vPosition), _Rotation(vRotation), _Scale(vScale)
		{
			__updateTranfrom();
		};

		operator const glm::mat4& () 
		{ 
			__updateTranfrom();
			return _Transform;
		}
		operator glm::mat4& () 
		{ 
			__updateTranfrom();
			return _Transform;
		}

		glm::mat4 getTransfrom()
		{
			__updateTranfrom();
			return _Transform;
		}

	private:

		glm::mat4 __calTransform(
			const glm::vec3& vTranslation,
			const glm::vec3& vRotation,
			const glm::vec3& vScale)
		{
			glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), vRotation.z, { 0.0f, 0.0f, 1.0f })
				* glm::rotate(glm::mat4(1.0f), vRotation.y, { 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), vRotation.x, { 1.0f, 0.0f, 0.0f });
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), vScale);
			glm::mat4 Translation = glm::translate(glm::mat4(1.0f), vTranslation);
			return Translation * Rotation * Scale;
		}

		void __updateTranfrom()
		{
			_Transform = __calTransform(_Position, glm::radians(_Rotation), _Scale);
		}

		glm::mat4 _Transform = glm::mat4(1.0);
	};
}