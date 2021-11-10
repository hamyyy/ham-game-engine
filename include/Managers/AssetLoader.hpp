#pragma once

namespace fs = std::filesystem;

namespace Ham::AssetLoader
{

static void ReadTextureFile(fs::path path);

static bool InvalidChar(char c)
{
    return !(c >= 0 && c < 128);
}
static std::string& StripUnicode(std::string& str)
{
    str.erase(remove_if(str.begin(), str.end(), InvalidChar), str.end());
    return str;
}

static std::string ReadShaderFile(fs::path path)
{

    if (!fs::exists(path))
        HAM_ASSERT(false, "Could not find file: \"" + path.string() + "\"");

    std::string result;

    std::fstream f(path, std::ios::in);
    std::string  line;

    if (f.is_open())
        while (std::getline(f, line))
            result += line + "\n";

    // StripUnicode(result);

    // std::cout << "Loading Shader...\n"
    //           << path << "\n\n\n"
    //           << result << std::endl;

    return result;
}

} // namespace Ham