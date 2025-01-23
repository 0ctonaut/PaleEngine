#include "Core/PaleRendererExport.h"

struct GLFWwindow;
namespace PaleRenderer
{
	class PALE_API CWindowOpenGL
	{
	public:
		CWindowOpenGL() = delete;
		CWindowOpenGL(const CWindowOpenGL&) = delete;

		CWindowOpenGL(const std::string& vName, unsigned int vWidth = 1920, unsigned int vHeight = 1080);
		~CWindowOpenGL();

		inline GLFWwindow* fetchWindowPtr() const { return m_pWindow; }

	private:
		GLFWwindow* m_pWindow;
	};
}