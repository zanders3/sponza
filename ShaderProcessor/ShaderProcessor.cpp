#include <iostream>
#include <fstream>

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

// Direct3D10 includes
#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dx10.h>

using namespace std;

bool endsWith(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() > ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

class EffectInclude : public ID3D10Include
{
public:
	EffectInclude(const string& localPath) : 
	  m_localPath(localPath) 
	{
	}

	STDMETHOD(Open)(D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
	{
		string filePath = m_localPath;
		filePath += string(pFileName);

		ifstream fs(filePath, ios::binary);
		if (fs.good())
		{
			fs.seekg(0, ios::end);
			size_t length = static_cast<size_t>(fs.tellg());
			fs.seekg(0, ios::beg);

			char * pData = new char[length];
			fs.read(pData, length);

			fs.close();

			*pBytes = length;
			*ppData = pData;

			return 0;
		}
		else 
			return 1;
	}

    STDMETHOD(Close)(LPCVOID pData)
	{
		char * pCData = (char*)pData;
		delete[] pCData;

		return 0;
	}

private:
	const string m_localPath;
};

int main(int argc, char ** argv)
{
	//Check arguments
	if (argc != 3)
	{
		cout << "Shader Processor\n\nUsage: ShaderProcessor.exe inputfile outputfile" << endl;
		cout << "Compiles .vsh and .psh shader files into shader bytecode" << endl;
		return 0;
	}

	//Validate input file type
	if (!endsWith(argv[1], ".fx"))
	{
		cout << "Unknown file format, expected *.fx : \"" << argv[1] << "\"" << endl;
		return 1;
	}

	//Read file
	char * data = nullptr;
	size_t length = 0;
	{
		ifstream filestr(argv[1], ios::binary);

		if (!filestr.good())
		{
			cout << "Failed to open file: \"" << argv[1] << "\"" << endl;
			return 1;
		}

		filestr.seekg(0, ios::end);
		length = static_cast<size_t>(filestr.tellg());
		filestr.seekg(0, ios::beg);

		data = new char[length];
		filestr.read(data, length);

		filestr.close();
	}

	if (length == 0)
	{
		cout << "File is empty!" << endl;
		return 1;
	}

	// Compile file
	ID3D10Blob* pCompiledShader;
	{
		std::string filePath = argv[1];
		size_t found = filePath.find_last_of("/\\");
		EffectInclude include(filePath.substr(0,found+1));

		UINT flags = D3D10_SHADER_WARNINGS_ARE_ERRORS;
#ifdef DEBUG
		flags |= D3D10_SHADER_DEBUG;
#else
		flags |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		ID3D10Blob* pErrorMessages;
		HRESULT hr = D3DX10CompileFromMemory(
			data, 
			length, 
			argv[1], 
			NULL, 
			&include, 
			NULL, 
			"fx_4_0",
			flags,
			NULL,
			NULL,
			&pCompiledShader,
			&pErrorMessages,
			NULL);

		if (FAILED(hr))
		{
			cout << "Error compiling shader: \"" << argv[1] << "\"" << endl;
			if (pErrorMessages)
			{
				char * data = (char*)pErrorMessages->GetBufferPointer();
				if (data)
				{
					cout << data << endl;
				}
			}
			return 1;
		}
	}

	//Save blob file
	{
		ofstream filestr(argv[2], ios::out | ios::binary);

		filestr.write((char*)pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize());

		filestr.close();
	}

	delete[] data;

	return 0;
}