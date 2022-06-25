#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "core.h"

#include "gfx/shader.h"
#include "utils/utils.h"

namespace utils
{
struct FeatureList
{
    std::vector<std::string> features;
    bool operator==(const FeatureList &other) const
    {
        if (features.size() != other.features.size()) return false;
        std::unordered_set<std::string> featureSet;
        for (auto &feature: features)
        {
            featureSet.insert(feature);
        }
        for (auto &feature: other.features)
        {
            if (featureSet.find(feature) == featureSet.end()) return false;
        }
        return true;
    }
};
} // namespace utils


namespace std 
{

template<>
struct hash<utils::FeatureList>
{
    std::size_t operator()(const utils::FeatureList &featureList) const
    {
        std::vector<size_t> hashes(featureList.features.size());
        for (int i = 0; i < featureList.features.size(); i++)
        {
            hashes[i] = std::hash<std::string>()(featureList.features[i]);
        }
        size_t outHash = 0;
        
        for (auto &hash: hashes)
        {
            outHash ^= hash + 0x9e3779b9 + (hash<<6) + (hash>>2);
        }

        return outHash;
    }
};


}  // namespace std

namespace utils
{

class ShaderLoader
{
public:
    ShaderLoader(std::string shaderPath);
    ShaderLoader() = default;
    ~ShaderLoader();

    void init(std::string shaderPath);
    bool has(std::string feature);
    void activate(std::string feature);
    void deactivate(std::string feature);
    bool state(std::string feature);
    std::unordered_map<std::string, bool> getFeatures();
    void setFeatures(std::unordered_map<std::string, bool> newfeatures);

    std::string load();

private:
    std::string getProcessedShaderCode();

    std::string shaderPath;
    std::unordered_map<std::string, bool> features;
    std::vector<std::string> lines;
    std::unordered_map<FeatureList, std::string> featureListToShader;
};

} // namespace utils


#endif
