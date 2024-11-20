#pragma once

namespace Client
{
	// ����ϴ� ����(��)�� ����
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_ENDING, LEVEL_END };

	// Client���� ����ϴ� ������Ʈ�� Ÿ��
	enum OBJECT_TYPE { OT_PLAYER, OT_CHAMPION, OT_ITEM, OT_END };

	// �迭 �ó���
	// ������, ����, ��İ�, ��������, ��������, ���� ����, �米��, �ð��¿�
	// ���̺�, ���, ��� ����, �뺴, ������, ȭ�а���, �渷

	// ���� �ó���
	// ��ȭ����, �Ž�, ��ȣ��, ������, ������, ��������, ����������
	// �ֹ���, �ϻ���, ���ݼ�, Ÿ�ݴ�, ����, ���Ű�
	enum SYNERGY {
		// �迭 �ó���
		Rival,			// ������
		Scrap,			// ����
		Glutton,		// ��İ�
		Mutant,			// ��������
		Hextech,		// ��������
		Syndicate,		// ���� ����
		Socialite,		// �米��
		Clockwork,		// �ð��¿�
		Debonair,		// ���̺�
		Yordle,			// ���
		Yordle_Lord,	// ��� ����
		Mercenary,		// �뺴
		Enforcer,		// ������
		Chemtech,		// ȭ�а���
		Mastermind,		// �渷

		// ���� �ó���
		Enchanter,		// ��ȭ����
		Colossus,		// �Ž�
		Bodyguard,		// ��ȣ��
		Bruiser,		// ������
		Challenger,		// ������
		Transformer,	// ��������
		Arcanist,		// ����������
		Twinshot,		// �ֹ���
		Assassin,		// �ϻ���
		Sniper,			// ���ݼ�
		Striker,		// Ÿ�ݴ�
		Scholar,		// ����
		Innovator,		// ���Ű�

		SYNERGY_END
	};

	// 1�� : 13
	// 2�� : 13
	// 3�� : 13
	// 4�� : 11
	// 5�� : 9
	enum ChampionName {
		// $5, 0 ~ 8
		Galio,  // ������
		Veigar, // ���̰�
		Viktor, // ���丣
		Silco,	// ����
		Zeri,	// ����
		Jayce,	// ���̽�
		Jinx,	// ¡ũ��
		Kaisa,	// ī�̻�
		Tahm_Kench,	// Ž ��ġ

		//$4, 9 ~ 20
		Draven,			// �巹�̺�
		Renata_Glask,	// ����Ÿ �۶�ũ
		Bye,			// ����	
		Brown,			// ����
		Seraphine,		// ������
		Sibir,			// �ú�
		Ari,			// �Ƹ�
		Alistar,		// �˸���Ÿ
		Auriana,		// �����Ƴ�
		Irelia,			// �̷�����
		Zin,			// ��
		Khazix,			// ī����
		
		//$3, 21 ~ 33
		Gangplank,		// ���÷�ũ
		Nar,			// ����
		Leona,			// ������
		Lucian,			// ��þ�
		Malzahar,		// ������
		Morgana,		// �𸣰���
		Miss_Fortune,	// �̽� ����
		Vecs,			// ����
		Sena,			// ����
		Echo,			// ����
		Zac,			// ��ũ
		Cho_Gath,		// �ʰ���
		Tryndamere,		// Ʈ���ٹ̾�

		//$2, 34 ~ 46
		Rek_Sai,		// ������
		Lulu,			// ���
		Blitzcrank,		// ����ũ��ũ
		Sejuani,		// ���־ƴ�
		Swain,			// ������
		Syndra,			// �ŵ��
		Ashe,			// �ֽ�
		Warwick,		// ����
		Zyra,			// ���̶�
		Zilean,			// ������
		Corki,			// �ڸ�Ű
		Quinn,			// ��
		Talon,			// Ż��

		//$1, 47 ~ 59
		Nocturne,	// ����
		Darius, 	// �ٸ��콺
		Brand,		// �귣��
		Poppy,		// �ǻ�
		Singed,		// ������
		Ezreal,		// �����
		Illaoi,		// �϶����
		Jarvan_IV,	// �ڸ��� 4��
		Ziggs,		// ����
		Camille,	// ī��
		Kassadin,	// ī���
		Caitlyn,	// ����Ʋ��
		Twitch,		// Ʈ��ġ

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
		// B.F. �ҵ�, ����, �㸮��, ��ū��, ����, ���, �尩, ������, ����
		BF_SWORD, CHAIN_VEST, GIANT_BELT, NEEDLESSLY_LARGE_ROD, NEGATRON_CLOAK, RECURVE_BOW, SPARRING_GLOVES, SPATULA, TEAR_OF_THE_GODDESS,

		// ����������, �ϻ���, ������, ȭ��, ���̺�, ��������, ��������, Ÿ�ݴ�, ��õ��, ����
		Arcanist_Emblem, Assassin_Emblen, Challenger_Emblem, Chemtech_Emblem, Debonair_Emblem, Hextech_Emblem, Mutant_Emblem, Striker_Emblem,

		// �ǹ�, ���, ����, ����, �װ�, ���, �㳡, ���, ������, ����
		Archangle_Staff, Banshee_Claw, Bloodthirster, Blue_Buff, Bramble_Vest, Chalice_Of_Power, Deathblade, Dragon_Claw, Edge_Of_Night, Frozen_Heart, Gargoyle_Stoneplate, Giant_Slayer,

		// ���μ�, ����, �Ѱ�, ����, �̿�, ����, ����, �ֶ�, ��, ����
		Guinsoo_Rageblade, Hand_Of_Justice, Hextech_Gunblade, Infinity_Edge, Lonic_Spark, Jeweled_Gauntlet, Last_Whisper, Locket_Of_The_Iron_Solari, Morellonomicon, Quicksilver,

		// ��ٵ�, ����, ����, �糭, ħ��, ����, ����ƽ, �ºҸ�, ������ �հ�, ����
		Rabadon_Deathcap, Rapid_Firecannon, Redemption, Runaan_Hurricane, Shroud_Of_Stillness, Spear_Of_Shoujin, Statikk_Shiv, Sunfire_Cape, Tacticain_Crown, Theif_Gloves,

		// �Ű�, �����, ��ũ, ��ǳ, ���
		Titan_Resolve, Warmog_Armor, Zeke_Herald, Zephyr, ZzRot_Portal,

		ITEM_END
	};
}