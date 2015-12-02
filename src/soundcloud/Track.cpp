/*
 * Copyright 2015 Adrián del Campo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Track.h"

namespace sc
{
	Track::Track()
	: m_id(0)
	, m_userId(0)
	, m_duration(0)
	, m_streamable(false)
	, m_downloadable(false)
	{ }

	Track::~Track()
	{ }

	void Track::loadFromJson(const Json::Value& json)
	{
		m_id = json["id"].asUInt();
		m_creationDate = json["created_at"].asString();
		m_userId = json["user_id"].asUInt();
		m_user = json["user"].toStyledString();
		m_title = json["title"].asString();

		m_permalink = json["permalink"].asString();
		m_permalinkurl = json["permalink_url"].asString();
		m_uri = json["uri"].asString();
		m_shareType = json["sharing"].asString();
		m_embeddable = json["embeddable_by"].asString();

		m_purchaseUrl = json["purchase_url"].asString();
		m_artUrl = json["artwork_url"].asString();
		m_description = json["description"].asString();
		m_label = json["label"].asString();

		m_duration = json["duration"].asUInt();
		m_genre = json["genre"].asString();
		m_tagList = json["tag_list"].asString();
		m_labelId = json["label_id"].asString();
		m_labelName = json["label_name"].asString();

		m_release = json["release"].asString();
		m_releaseDay = json["release_day"].asString();
		m_releaseMonth = json["release_month"].asString();
		m_releaseYear = json["release_year"].asString();

		m_streamable = json["streamable"].asBool();
		m_downloadable = json["downloadable"].asBool();
		m_state = json["state"].asString();
		m_license = json["license"].asString();
		m_trackType = json["track_type"].asString();
		m_waveUrl = json["waveform_url"].asString();

		m_downloadUrl = json["download_url"].asString();
		m_streamUrl = json["stream_url"].asString();
		m_videoUrl = json["video_url"].asString();
		m_bpm = json["bpm"].asString();
	}
} /* namespace sc */
