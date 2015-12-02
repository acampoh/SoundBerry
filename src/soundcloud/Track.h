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

#ifndef SRC_TRACK_H_
#define SRC_TRACK_H_

#include <string>
#include "../json/json.h"

namespace sc
{
	//TODO: Create classes to manage this items. Temporary workaround;
	typedef std::string User; //Future class
	typedef std::string Sharing; //Future enum;
	typedef std::string State; //Future enum;
	typedef std::string EmbeddableBy; //Future enum;
	typedef std::string Label; //Future class;
	typedef std::string License; //Future class;
	typedef std::string TrackType; //Future enum;

	class Track
	{
	public:
		Track();
		virtual ~Track();

		void loadFromJson(const Json::Value& json);

		std::string getStreamUrl() const { return m_streamUrl; }
	private:
		uint32_t m_id;
		std::string m_creationDate;
		uint32_t m_userId;
		User m_user;
		std::string m_title;
		std::string m_permalink;
		std::string m_permalinkurl;
		std::string m_uri;
		Sharing m_shareType;
		EmbeddableBy m_embeddable;
		std::string m_purchaseUrl;
		std::string m_artUrl;
		std::string m_description;
		Label m_label;
		uint32_t m_duration;
		std::string m_genre;
		std::string m_tagList;
		std::string m_labelId;
		std::string m_labelName;
		std::string m_release;
		std::string m_releaseDay;
		std::string m_releaseMonth;
		std::string m_releaseYear;
		bool m_streamable;
		bool m_downloadable;
		State m_state;
		License m_license;
		TrackType m_trackType;
		std::string m_waveUrl;
		std::string m_downloadUrl;
		std::string m_streamUrl;
		std::string m_videoUrl;
		std::string m_bpm;
	};
} /* namespace sc */

#endif /* SRC_TRACK_H_ */
