#include "utils/shader_loader.h"


namespace utils
{

std::string getFeatureFromLine(std::string line)
{
    auto feature_pos = line.find("#ifdef ");
    line.erase(feature_pos, std::string("#ifdef ").size());
    return line;
}
std::string sanitise(std::string feature)
{
    std::string sanitised_feature = feature;
    sanitised_feature.erase(std::remove(sanitised_feature.begin(), sanitised_feature.end(), ' '), sanitised_feature.end());
    return sanitised_feature;
}

ShaderLoader::ShaderLoader(std::string shaderPath) : shaderPath(shaderPath)
{
    std::string shaderFile = read_file(shaderPath.c_str());

    std::istringstream file{shaderFile};
    std::string line;

    int features_count = 0;

    while (std::getline(file, line))
    {
        lines.push_back(line);
        auto feature_pos = line.find("#ifdef ");
        if (feature_pos != std::string::npos)
        {
            ASSERT(features_count == 0, "nested ifs are not implimented");
            features_count++;
            std::string feature = sanitise(getFeatureFromLine(line));
            features[feature] = false;
        }
        if (line.find("#else") != std::string::npos)
        {
            ASSERT(features_count > 0, "unexpected else");
        }
        if (line.find("#endif") != std::string::npos)
        {
            ASSERT(features_count > 0, "unexpected endfeature");
            features_count--;
        }

    }   
}

void ShaderLoader::init(std::string shaderPath)
{
    ShaderLoader::shaderPath = shaderPath;
    std::string shaderFile = read_file(shaderPath.c_str());

    std::istringstream file{shaderFile};
    std::string line;

    int features_count = 0;

    while (std::getline(file, line))
    {
        lines.push_back(line);
        auto feature_pos = line.find("#ifdef ");
        if (feature_pos != std::string::npos)
        {
            ASSERT(features_count == 0, "nested ifs are not implimented");
            features_count++;
            std::string feature = sanitise(getFeatureFromLine(line));
            features[feature] = false;
        }
        if (line.find("#else") != std::string::npos)
        {
            ASSERT(features_count > 0, "unexpected else");
        }
        if (line.find("#endif") != std::string::npos)
        {
            ASSERT(features_count > 0, "unexpected endfeature");
            features_count--;
        }

    }   
}

ShaderLoader::~ShaderLoader()
{

}

bool ShaderLoader::has(std::string feature)
{
    return (features.find(feature) != features.end());
}

void ShaderLoader::activate(std::string feature)
{
    ASSERT(has(feature), "shader does not have feature: " + feature);
    features.at(feature) = true;
}

void ShaderLoader::deactivate(std::string feature)
{
    ASSERT(has(feature), "shader does not have feature: " + feature);
    features.at(feature) = false;
}

bool ShaderLoader::state(std::string feature)
{
    ASSERT(has(feature), "shader does not have feature: " + feature);
    return features.at(feature);
}

std::unordered_map<std::string, bool> ShaderLoader::getFeatures()
{
    return features;
}

void ShaderLoader::setFeatures(std::unordered_map<std::string, bool> newfeatures)
{
    ASSERT(features.size() == ShaderLoader::features.size(), "given feature map is not equivalent to the internal feature map");
    for (auto &feature: newfeatures)
    {
        auto featureLoc = features.find(feature.first);
        ASSERT(featureLoc != features.end(), "given feature map contains features not in the intarnal feature map");
        featureLoc->second = feature.second;
    }
}

std::string ShaderLoader::getProcessedShaderCode()
{
    std::string shaderCode = "";
    bool disregard = false;
    for (auto& line: lines)
    {
        if (line.find("#ifdef ") == std::string::npos)
        {
            if (line.find("#else") == std::string::npos)
            {
                if (line.find("#endif") == std::string::npos)
                {
                    if (!disregard)
                        shaderCode += line + '\n';
                }
                else
                {
                    disregard = false;
                }
            }    
            else
            {
                if (disregard)
                    disregard = false;
                else
                    disregard = true;                 
            }                
        }
        else
        {
            std::string feature = sanitise(getFeatureFromLine(line));
            auto feature_pos = features.find(feature);
            ASSERT(feature_pos != features.end(), "feature not in features");
            if (feature_pos->second)
            {
                disregard = false;
            }
            else
            {
                disregard = true;
            }

        }
    }
    return shaderCode;
}

std::string ShaderLoader::load()
{
    FeatureList featureList;
    for (auto &feature: features)
    {
        if (feature.second) featureList.features.push_back(feature.first);
    }
    auto shaderProgram = featureListToShader.find(featureList);
    if (shaderProgram != featureListToShader.end()) return shaderProgram->second;
    std::string shaderCode = getProcessedShaderCode();

    featureListToShader[featureList] = shaderCode;
    return shaderCode;
}

} // namespace utils
