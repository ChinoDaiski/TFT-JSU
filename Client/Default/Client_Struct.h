#pragma once

namespace Client
{
	// 사용하는 레벨(씬)의 종류
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_ENDING, LEVEL_END };

	// Client에서 사용하는 오브젝트의 타입
	enum OBJECT_TYPE { OT_PLAYER, OT_CHAMPION, OT_ITEM, OT_END };

	// 계열 시너지
	// 경쟁자, 고물상, 대식가, 돌연변이, 마법공학, 범죄 조직, 사교계, 시계태엽
	// 연미복, 요들, 요들 군주, 용병, 집행자, 화학공학, 흑막

	// 직업 시너지
	// 강화술사, 거신, 경호대, 난동꾼, 도전자, 변형술사, 비전마법사
	// 쌍발총, 암살자, 저격수, 타격대, 학자, 혁신가
	enum SYNERGY {
		// 계열 시너지
		Rival,			// 경쟁자
		Scrap,			// 고물상
		Glutton,		// 대식가
		Mutant,			// 돌연변이
		Hextech,		// 마법공학
		Syndicate,		// 범죄 조직
		Socialite,		// 사교계
		Clockwork,		// 시계태엽
		Debonair,		// 연미복
		Yordle,			// 요들
		Yordle_Lord,	// 요들 군주
		Mercenary,		// 용병
		Enforcer,		// 집행자
		Chemtech,		// 화학공학
		Mastermind,		// 흑막

		// 직업 시너지
		Enchanter,		// 강화술사
		Colossus,		// 거신
		Bodyguard,		// 경호대
		Bruiser,		// 난동꾼
		Challenger,		// 도전자
		Transformer,	// 변형술사
		Arcanist,		// 비전마법사
		Twinshot,		// 쌍발총
		Assassin,		// 암살자
		Sniper,			// 저격수
		Striker,		// 타격대
		Scholar,		// 학자
		Innovator,		// 혁신가

		SYNERGY_END
	};

	// 1렙 : 13
	// 2렙 : 13
	// 3렙 : 13
	// 4렙 : 11
	// 5렙 : 9
	enum ChampionName {
		// $5, 0 ~ 8
		Galio,  // 갈리오
		Veigar, // 베이가
		Viktor, // 빅토르
		Silco,	// 실코
		Zeri,	// 제리
		Jayce,	// 제이스
		Jinx,	// 징크스
		Kaisa,	// 카이사
		Tahm_Kench,	// 탐 켄치

		//$4, 9 ~ 20
		Draven,			// 드레이븐
		Renata_Glask,	// 레나타 글라스크
		Bye,			// 바이	
		Brown,			// 브라움
		Seraphine,		// 세라핀
		Sibir,			// 시비르
		Ari,			// 아리
		Alistar,		// 알리스타
		Auriana,		// 오리아나
		Irelia,			// 이렐리아
		Zin,			// 진
		Khazix,			// 카직스
		
		//$3, 21 ~ 33
		Gangplank,		// 갱플랭크
		Nar,			// 나르
		Leona,			// 레오나
		Lucian,			// 루시안
		Malzahar,		// 말자하
		Morgana,		// 모르가나
		Miss_Fortune,	// 미스 포츈
		Vecs,			// 벡스
		Sena,			// 세나
		Echo,			// 에코
		Zac,			// 자크
		Cho_Gath,		// 초가스
		Tryndamere,		// 트린다미어

		//$2, 34 ~ 46
		Rek_Sai,		// 렉사이
		Lulu,			// 룰루
		Blitzcrank,		// 블리츠크랭크
		Sejuani,		// 세주아니
		Swain,			// 스웨인
		Syndra,			// 신드라
		Ashe,			// 애쉬
		Warwick,		// 워윅
		Zyra,			// 자이라
		Zilean,			// 질리언
		Corki,			// 코르키
		Quinn,			// 퀸
		Talon,			// 탈론

		//$1, 47 ~ 59
		Nocturne,	// 녹턴
		Darius, 	// 다리우스
		Brand,		// 브랜드
		Poppy,		// 뽀삐
		Singed,		// 신지드
		Ezreal,		// 이즈리얼
		Illaoi,		// 일라오이
		Jarvan_IV,	// 자르반 4세
		Ziggs,		// 직스
		Camille,	// 카밀
		Kassadin,	// 카사딘
		Caitlyn,	// 케이틀린
		Twitch,		// 트위치

		ChampionName_END
	};

	enum CharacterName {
		Character_Pet_Aoshin,
		Character_Pet_Choncc,
		Character_Pet_Sgcat,
		Character_Pet_Umbra,
		Character_Pet_END
	};

	enum ITEM_NAME {
		// B.F. 소드, 조끼, 허리띠, 쓸큰지, 망토, 곡궁, 장갑, 뒤집개, 여눈
		BF_SWORD, CHAIN_VEST, GIANT_BELT, NEEDLESSLY_LARGE_ROD, NEGATRON_CLOAK, RECURVE_BOW, SPARRING_GLOVES, SPATULA, TEAR_OF_THE_GODDESS,

		// 비전마법사, 암살자, 도전자, 화공, 연미복, 마법공학, 돌연변이, 타격대, 대천사, 벤시
		Arcanist_Emblem, Assassin_Emblen, Challenger_Emblem, Chemtech_Emblem, Debonair_Emblem, Hextech_Emblem, Mutant_Emblem, Striker_Emblem,

		// 피바, 블루, 가갑, 성배, 죽검, 용발, 밤끝, 얼심, 가고일, 거학
		Archangle_Staff, Banshee_Claw, Bloodthirster, Blue_Buff, Bramble_Vest, Chalice_Of_Power, Deathblade, Dragon_Claw, Edge_Of_Night, Frozen_Heart, Gargoyle_Stoneplate, Giant_Slayer,

		// 구인수, 정손, 총검, 무대, 이온, 보건, 라위, 솔라리, 모렐, 수인
		Guinsoo_Rageblade, Hand_Of_Justice, Hextech_Gunblade, Infinity_Edge, Lonic_Spark, Jeweled_Gauntlet, Last_Whisper, Locket_Of_The_Iron_Solari, Morellonomicon, Quicksilver,

		// 라바돈, 고연포, 구원, 루난, 침장, 쇼진, 스태틱, 태불망, 전략가 왕관, 도장
		Rabadon_Deathcap, Rapid_Firecannon, Redemption, Runaan_Hurricane, Shroud_Of_Stillness, Spear_Of_Shoujin, Statikk_Shiv, Sunfire_Cape, Tacticain_Crown, Theif_Gloves,

		// 거결, 워모그, 지크, 서풍, 즈롯
		Titan_Resolve, Warmog_Armor, Zeke_Herald, Zephyr, ZzRot_Portal,

		ITEM_END
	};
}