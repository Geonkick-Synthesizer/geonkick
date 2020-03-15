#include "kit_state.h"

KitState::KitState()
{
}

bool KitState::open(const std::string &file)
{
        if (file.size() < 6) {
                RK_LOG_ERROR("can't open preset. File name empty or wrong format.");
                return false;
        }

        std::filesystem::path filePath(file);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkit"
            && filePath.extension() != ".GKIT")) {
                RK_LOG_ERROR("can't open kit. Wrong file format.");
                return false;
        }

        std::ifstream sfile;
        sfile.open(std::filesystem::absolute(filePath));
        if (!sfile.is_open()) {
                RK_LOG_ERROR("can't open kit.");
                return false;
        }
        std::string fileData((std::istreambuf_iterator<char>(sfile)),
                             (std::istreambuf_iterator<char>()));
        sfile.close();
        parseKit(fileData, filePath.parent_path());
}

void KitWidget::parseKit(std::string &fileData, const std::filesystem::path &path)
{
        rapidjson::Document document;
        Kit kit;
        document.Parse(fileData.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                kitName = m.value.GetString();
                        if (m.name == "author" && m.value.IsString())
                                kitAuthor = m.value.GetString();
                        if (m.name == "url" && m.value.IsString())
                                kitUrl = m.value.GetString();
                        if (m.name == "percussions" && m.value.IsArray())
                                parsePercussions(m.value, path);
                }
        }
}

std::vector<KitWidget::Percussion>
KitWidget::parsePercussions(const rapidjson::Value &envelopeArray,
                            const std::filesystem::path &path)
{
        for (const auto &el: envelopeArray.GetArray()) {
                if (el.IsObject()) {
                        auto per = std::make_shared<GeonkickState>();
                        for (const auto &m: el.GetObject()) {
                                if (m.name == "id" && m.value.IsInt())
                                        per.setId(m.value.GetInt());
                                if (m.name == "name" && m.value.IsString())
                                        per.setKickName(m.value.GetString());
                                if (m.name == "file" && m.value.IsString())
                                        per.loadFile(path / std::filesystem::path(m.value.GetString()));
                                if (m.name == "key" && m.value.IsInt())
                                        per.setPlayingKey(m.value.GetInt());
                                if (m.name == "enabled" && m.value.IsBool())
                                        per.enable(m.value.GetBool());
                        }
                        percussionsList.push_back(std::move(per));
                }
        }
}

bool KitState::save(const std::string &file)
{
        if (fileName.size() < 6) {
                RK_LOG_ERROR("can't save kit. Wrong file name");
                return false;
        }

        std::filesystem::path filePath(file);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkit"
            && filePath.extension() != ".GKIT"))
                filePath.replace_extension(".gkit");

        std::ofstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("can't open file for saving: " << file);
                return false;
        }
        std::ostringstream stream;
        getKitObject(stream);
        file << stream;
        file.close();

        std::filesystem::path filePath(file);
        auto path = filePath.has_parent_path() ? filePath.parent_path() : filePath;
        for (auto &per: percussionList)
                per->save(path / std::filesystem::path(per->getName() + ".gkick"));

        return true;
}

std::ostringstream
KitState::getKitObject() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
	jsonStream <<  "\"name\": \"" << kitName << "\"," << std::endl;
        jsonStream <<  "\"author\": \"" << kitAuthor << "\"," << std::endl;
        jsonStream <<  "\"url\": \"" << kitUrl << "\"," << std::endl;
        jsonStream <<  "\"percussions\": [" << std::endl;
        for (auto i = 0; i < percussionsList.size(); i++) {
                jsonStream << "{" << std::endl;
		jsonStream <<	"\"id\": " << percussionsList[i]->getId() << "," << std::endl;
                jsonStream <<	"\"name\": \"" << percussionsList[i]->getName() << "\"," << std::endl;
                jsonStream <<	"\"file\": \"" << percussionsList[i]->getName() << ".gkick\"," << std::endl;
                jsonStream <<	"\"key\": " << percussionsList[i]->getPlayingKey() << "," << std::endl;
                jsonStream <<	"\"enabled\": " << (percussionsList[i]->isKickEnabled() ? "true" : "false") << std::endl;
                jsonStream << "}" << (i < percussionsList.size() ? "," : "") << std::endl;
        }
        jsonStream <<  "]" << std::endl;
        jsonStream <<  "}" << std::endl;
        return jsonStream;
}

void KitState::setName(const std::string &name)
{
        kitName = name;
}

std::string KitState::getName() const
{
        return kitName;
}

void KitState::setAuthor(const std::string &author)
{
        kitAuthor = author;
}

std::string KitState::getAuthor() const
{
        return kitAuthor;
}

void KitState::setUrl(const std::string &url)
{
        kitUrl = url;
}

std::string KitState::getUrl() const
{
        return kitUrl;
}

std::vector<std::shared_ptr<GeonkickState>>& percussions() const
{
        return percussionsList;
}

bool KitState::fromJson(const std::string &jsonData)
{
        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "kit" && m.value.IsObject())
                                kitName = m.value.GetString();
                        if (m.name == "author" && m.value.IsString())
                                kitAuthor = m.value.GetString();
                        if (m.name == "url" && m.value.IsString())
                                kitUrl = m.value.GetString();
                        if (m.name == "percussions" && m.value.IsArray())
                                parsePercussions(m.value, path);
                }
        }

}

std::string KitState::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << "{" << std::endl;
        jsonStream << "\"kit\":" << std::endl;
        jsonStream <<  getKitObject() << "," << std::endl;
        jsonStream <<  "\"percussions\": [" << std::endl;
        for (auto i = 0; i < percussionsList.size(); i++) {
                if (i < percussionsList.size() - 1)
                        std::ostringstream << percussionsList[i]->toJson() << "," << std::endl;
                else
                        std::ostringstream << percussionsList[i]->toJson();
        }
        jsonStream <<  "]" << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

void KitState::addPercussion(std::unique_ptr<GeonkickState> &percussion)
{
        percussionsList.push_back(std::move(percussion));
}
