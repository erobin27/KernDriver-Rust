#pragma once
#include "myMath.h"
#include "settings.h"
#include "GStructs.h"
#include "UM-Memory.h"

//game structs
//[Base + g_BN_Steam] + oEntityRealm] + oClientEntitiesList] + oClientEntitiesValues]
//client entities = read()
#define g_BN 0x3171BE0			//0x32A4668
#define g_BN_Steam 0x3171BE0
#define GOM 0x17C1F18			//doesn't change (usually)


#define oEntityRealm 0xB8
#define oClientEntitiesList 0x10
#define oClientEntitiesValues 0x28
#define oEntityCount 0x10 ////clientValues + EntityCount
#define oEntityBuffer 0x18 //clientValues + EntityBuffer

//BaseCombatEntity Class
#define oPlayerHealth 0x224		//private float _health;
#define oMaxHealth 0x228		//float _maxHealth

//BaseEntity Class
#define oModel 0x128			//BaseEntity: Model model

//Model Class
#define oBoneTransforms 0x48


//BasePlayer Class
#define oPlayerInput 0x4E0		//BasePlayer: PlayerInput input
#define oPlayerMovement 0x4E8	//BasePlayer: BaseMovement movement
#define oPlayerModel 0x4C0		//BasePlayer: PlayerModel playerModel
#define oWasSleeping 0x521		//BasePlayer: Bool wasSleeping				not used
#define oWasDead 0x523			//BasePlayer: Bool wasDead
#define oCurrentTeam 0x590		//BasePlayer: ULONG Current Team
#define oClientTeam 0x598		//BasePlayer: PlayerTeam clientTeam
#define oActiveUID 0x5C8		//BasePlayer: uint clActiveItem;
#define oModelState 0x5F0		//BasePlayer: modelstate modelstate;
#define oPlayerFlags 0x678		//BasePlayer: BasePlayer.PlayerFlags playerFlags
#define oPlayerInventory 0x688	//BasePlayer: PlayerInvetory inventory
#define oBelt 0x6b0				//BasePlayer: PlayerBelt Belt
#define oSteamID 0x6c0			//BasePlayer: ULONG UserID
#define oDisplayName 0x6d8		//BasePlayer: string _displayName

//PlayerInventory Class
#define oContainerMain 0x20		//PlayerInventory: ItemContainer containerMain
#define oContainerBelt 0x28		//PlayerInventory: ItemContainer containerBelt
#define oContainerWear 0x30		//PlayerInventory: ItemContainer containerWear

//ItemContainer Class
#define oUID 0x2c				//ItemContainer: uint uid
#define oItemList 0x38			//ItemContainer: List(item) itemList

//Item Class
#define oHeld 0x98				//Item:	EntityRef heldEntity

//PlayerModel Class
#define oSmoothLookAngle 0x20	//PlayerModel: Vector3 SmoothLookAngle
#define oPlayerPosition 0x218	//PlayerModel: Vector3 position

//BaseProjectile Class			Referenced by oHeld
#define oRecoil 0x2D8			//BaseProjectile: RecoilProperties recoil;
#define oAimCone 0x2E8			//BaseProjectile: float aimCone;
#define oHipAimCone 0x2EC		//BaseProjectile: float hipAimCone;
#define oAimconePenaltyPerShot 0x2F0	//BaseProjectile: float aimconePenaltyPerShot;
#define oAimconePenaltyMax 0x2F4		//BaseProjectile: float hipAimCone;
#define oStancePenalty 0x300 	//BaseProjectile: float stancePenalty;
#define oAimConePenalty 0x320 	//BaseProjectile: float aimconePenalty;		//increases from 0 to 1.0 as you fire


//RecoilProperties Class
#define oRecoilMinYaw 0x18		//RecoilProperties: float recoilYawMin
#define oRecoilMaxYaw 0x1C		//RecoilProperties: float recoilYawMax
#define oRecoilMinPitch 0x20	//RecoilProperties: float recoilPitchMin
#define oRecoilMaxPitch 0x24	//RecoilProperties: float recoilPitchMax
#define oADSScale 0x30			//RecoilProperties: float ADSScale
#define oMovementPenalty 0x34	//RecoilProperties: float movementPenalty;

//ModelState Class
#define oModelFlags 0x24	//int enum ModelFlags

//ArrayList Class				used for lists like oItemList
#define oArrayListItems 0x10			//ArrayList: object[] 0x10
#define oArrayListSize	0x18			//ArrayList: int 0x18


//not used
#define oNoBlockAiming 0x6B0	//public bool clothingBlocksAiming; //IDK THIS OFFSET
#define oSpeedReduction 0x6B4	//	public float clothingMoveSpeedReduction;
#define oMagazine 0x2A0 
#define oVisible 0x248			// internal bool visible;
#define oNoAimSway 0x6BC		//	public float clothingAccuracyBonus;
#define oLifestate 0x204		//	public BaseCombatEntity.LifeState lifestate;
#define oAuto 0x270   // public bool automatic;
#define oDistance 0x278 //private Transform attachmentBoneCache; public float maxDistance; // 0x278
#define oAttackRadius 0x27C //public float attackRadius;
#define oVelocity 0x1FC  // 	private Vector3 newVelocity;
#define oFakeAdmin 0x5F8   //public BasePlayer.PlayerFlags playerFlags;
#define oSuccessFraction 0x340  //public float successFraction;
#define oGroundAngle 0xC4 // private float groundAngle;
#define oGroundAngleNew 0xC8  // private float groundAngleNew;
#define oGravity 0x74  //public float gravityMultiplier;
#define oWaterBoost 0x6B8   //	public float clothingWaterSpeedBonus;
#define oFlyHack  0x13C     // private bool flying;
#define oMultiplierSwimming 0x78  // public float gravityMultiplierSwimming;
#define oTickTime 0x5CC //	private float lastSentTickTime;
#define oFrozen 0x4B0 // public bool Frozen;
#define oKeyFocus 0x94  // private bool hasKeyFocus;
#define oClientTickInterval 0x5C8  // public float clientTickInterval;
//#define oNoClip  0x563900     //public static void noclip(ConsoleSystem.Arg arg) { }

enum PlayerFlags {
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	VoiceMuted = 512,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
};

namespace ModelState {
	enum ModelFlags {
		Ducked = 1,
		Jumped = 2,
		OnGround = 4,
		Sleeping = 8,
		Sprinting = 16,
		OnLadder = 32,
		Flying = 64,
		Aiming = 128,
		Prone = 256,
		Mounted = 512,
		Relaxed = 1024,
		OnPhone = 2048,
		Crawling = 4096,
		HeadLock = 16384
	};
};
const unsigned short Crc16Table[256] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

enum BoneList : int
{
	pelvis = 0,
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	penis = 6,
	GenitalCensor = 7,
	GenitalCensor_LOD0 = 8,
	Inner_LOD0 = 9,
	GenitalCensor_LOD1 = 11,
	GenitalCensor_LOD2 = 12,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	r_ankle_scale = 17,
	spine1 = 18,
	spine1_scale = 19,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_clavicle = 23,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	l_index1 = 27,
	l_index2 = 28,
	l_index3 = 29,
	l_little1 = 30,
	l_little2 = 31,
	l_little3 = 32,
	l_middle1 = 33,
	l_middle2 = 34,
	l_middle3 = 35,
	l_prop = 36,
	l_ring1 = 37,
	l_ring2 = 38,
	l_ring3 = 39,
	l_thumb1 = 40,
	l_thumb2 = 41,
	l_thumb3 = 42,
	IKtarget_righthand_min = 43,
	IKtarget_righthand_max = 44,
	l_ulna = 45,
	neck = 46,
	head = 47,
	jaw = 48,
	eyeTranform = 49,
	l_eye = 50,
	l_Eyelid = 51,
	r_eye = 52,
	r_Eyelid = 53,
	r_clavicle = 54,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
	r_index1 = 58,
	r_index2 = 59,
	r_index3 = 60,
	r_little1 = 61,
	r_little2 = 62,
	r_little3 = 63,
	r_middle2 = 65,
	r_middle3 = 66,
	r_prop = 67,
	r_ring1 = 68,
	r_ring2 = 69,
	r_ring3 = 70,
	r_thumb1 = 71,
	r_thumb2 = 72,
	r_thumb3 = 73,
	IKtarget_lefthand_min = 74,
	IKtarget_lefthand_max = 75,
	r_ulna = 76,
	l_breast = 77,
	r_breast = 78,
	BoobCensor = 79,
	BreastCensor_LOD0 = 80,
	BreastCensor_LOD1 = 83,
	BreastCensor_LOD2 = 84
};


class EntityClass {
public:
	DWORD64 Entity;
	DWORD64 ObjectClass;
	std::string name;
	int EntityType;
	Vector3 position;

	EntityClass(DWORD64 EntityAddr, DWORD64 ObjectClassAddr, std::string name, int type) {
		Entity = EntityAddr;
		ObjectClass = ObjectClassAddr;
		this->name = name;
		EntityType = type;
		setPosition();
	}

	enum EntityTypes {
		Lootbox = 0,
		Ore = 1,
	};

	void setPosition() {
		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) return;
		DWORD64 LocalObjectClass = mem->Read<DWORD64>(Object + 0x30);
		if (LocalObjectClass <= 100000) return;
		DWORD64 m_objptr = mem->Read<DWORD64>(LocalObjectClass + 0x30);
		DWORD64 m_visual = mem->Read<DWORD64>(m_objptr + 0x8);
		DWORD64 m_pos_comp = mem->Read<DWORD64>(m_visual + 0x38);
		position = mem->Read<Vector3>(m_pos_comp + 0x90);
	}

	bool isActive() {
		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) return false;
		DWORD64 LocalObjectClass = mem->Read<DWORD64>(Object + 0x30);
		if (LocalObjectClass <= 100000) return false;
		DWORD64 m_objptr = mem->Read<DWORD64>(LocalObjectClass + 0x30);
		DWORD64 idk = mem->Read<DWORD64>(m_objptr + 0x18);
		BYTE b1 = mem->Read<BYTE>(idk + 0x38);
		BYTE b2 = mem->Read<BYTE>(idk + 0x39);
		return (b1 && b2);	//if b1 and b2 are both 1 it is active both 0 inactive
	}

	enum boxType {
		hiddenhackablecrate = 0,
		crate_basic = 1,
		crate_elite = 2,
		crate_mine = 3,
		crate_normal = 4,
		crate_normal_2 = 5,
		crate_normal_2_food = 6,
		crate_normal_2_medical = 7,
		crate_tools = 8,
		crate_underwater_advanced = 9,
		crate_underwater_basic = 10,
		crate_ammunition = 11,
		crate_food_1 = 12,
		crate_food_2 = 13,
		crate_fuel = 14,
		crate_medical = 15,
		codelockedhackablecrate = 16,
		codelockedhackablecrate_oilrig = 17,
		bradley_crate = 18,
		heli_crate = 19,
		cratemarker = 20,
		survey_crater = 21,
		survey_crater_oil = 22,
	};

	enum OreType {
		stone_ore = 0,
		metal_ore = 1,
		sulfur_ore = 2,
	};
};

class WeaponData
{
private:
	unsigned short CRC(unsigned char* pcBlock, unsigned short len)
	{
		unsigned short crc = 0xFFFF;
		while (len--)
			crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *pcBlock++];
		return crc;
	}

public:
	std::wstring readWCharString(DWORD64 address, int length) {
		std::wstring str = L"";
		for (int i = 0; i < length; i++) {
			//std::wcout << mem->Read<UCHAR>(address + i * 2);
			str += mem->Read<wchar_t>(address + i*2);
		}

		return str;
	}


	std::wstring GetShortName(int* len)
	{
		DWORD64 Info = mem->Read<DWORD64>((UINT_PTR)this + 0x20);
		DWORD64 Str = mem->Read<DWORD64>(Info + 0x20);

		int leng = mem->Read<int>(Str + 0x10);
		//printf("\nLeng: %i\n", leng);
		if (!leng) return nullptr;
		if (len)*len = leng;
		//printf("Return: %S\n", readWCharString(Str + 0x14, leng).c_str());
		return readWCharString(Str + 0x14, leng);
	}

	std::wstring GetNameHash()
	{
		int Len;
		std::wstring ShortName = GetShortName(&Len);
		if (ShortName.empty())
			return L"";

		//printf("\nShort Name: %S\n", ShortName.c_str());

		return ShortName;
	}

	wchar_t* GetName()
	{
		DWORD64 Info = mem->Read<DWORD64>((UINT_PTR)this + 0x20); // private string _targetName;
		DWORD64 DispName = mem->Read<DWORD64>(Info + 0x28); // public string TargetName;
		pUncStr Str = ((pUncStr)mem->Read<DWORD64>(DispName + 0x18)); // private string _TargetNamespace;
		if (!Str)
			return nullptr;
		return Str->str;
	}
	int GetID() {
		DWORD64 Info = mem->Read<DWORD64>((DWORD64)this + 0x20);
		int ID = mem->Read<int>(Info + 0x18);
		return ID;
	}
	DWORD64 GetUID()
	{
		DWORD64 uid = mem->Read<DWORD64>((UINT_PTR)this + 0x28); // public uint uid
		return uid;
	}

	void dumpDefaultRecoilVals() {
		DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
		DWORD64 recoil = mem->Read<DWORD64>(Held + oRecoil); //public RecoilProperties recoil;

		std::cout << mem->Read<float>(recoil + oRecoilMinYaw) << ", " << mem->Read<float>(recoil + oRecoilMaxYaw) << ", "
			<< mem->Read<float>(recoil + oRecoilMinPitch) << ", " << mem->Read<float>(recoil + oRecoilMaxPitch) << ", "
			<< mem->Read<float>(recoil + oADSScale) << ", " << mem->Read<float>(recoil + oMovementPenalty) << std::endl;
	}

	void NoRecoil()
	{
		if (true)
		{
			//printf("Recoil");
			DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
			DWORD64 recoil = mem->Read<DWORD64>(Held + oRecoil); //public RecoilProperties recoil;

			float recoilVal = -8.0f;
			if (!mem->write<float>((recoil + oRecoilMinYaw), recoilVal)) return; //public float recoilYawMin;
			if (!mem->write<float>((recoil + oRecoilMaxYaw), recoilVal)) return; //public float recoilYawMax;
			if (!mem->write<float>((recoil + oRecoilMinPitch), recoilVal)) return; //public float recoilPitchMin;
			if (!mem->write<float>((recoil + oRecoilMaxPitch), recoilVal)) return; //public float recoilPitchMax;
			if (!mem->write<float>((recoil + oADSScale), 0.3f)) return; //public float ADSScale;
			if (!mem->write<float>((recoil + oMovementPenalty), recoilVal)) return; //public float movementPenalty;
		}
	}

	/*
	*	Returns as so...
	*	0: MinYaw
	*	1: MaxYaw
	*	2: MinPitch
	*	3: MaxPitch
	*	4: ADS Scale
	*	5: Movement Penalty
	*/
	float* ReadRecoil() {
		float* dArr = new float[6];
		DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
		DWORD64 recoil = mem->Read<DWORD64>(Held + oRecoil); //public RecoilProperties recoil;
		std::cout << "\n\n-----READING MinYaw: " << mem->Read<float>((recoil + oRecoilMinYaw));
		std::cout << "\n\n-----READING MaxYaw: " << mem->Read<float>((recoil + oRecoilMaxYaw));
		std::cout << "\n\n-----READING MinPitch: " << mem->Read<float>((recoil + oRecoilMinPitch));
		std::cout << "\n\n-----READING MaxPitch: " << mem->Read<float>((recoil + oRecoilMaxPitch));
		std::cout << "\n\n-----READING ADS Scale: " << mem->Read<float>((recoil + oADSScale));
		std::cout << "\n\n-----READING Move Pen: " << mem->Read<float>((recoil + oMovementPenalty));
		dArr[0] = mem->Read<float>((recoil + oRecoilMinYaw));
		dArr[1] = mem->Read<float>((recoil + oRecoilMaxYaw));
		dArr[2] = mem->Read<float>((recoil + oRecoilMinPitch));
		dArr[3] = mem->Read<float>((recoil + oRecoilMaxPitch));
		dArr[4] = mem->Read<float>((recoil + oADSScale));
		dArr[5] = mem->Read<float>((recoil + oMovementPenalty));
		return dArr;
	}

	void NoRecoil(float minYaw, float maxYaw, float minPitch, float maxPitch, float ADS, float movePen)
	{
		std::cout << "minYaw: " << minYaw << "\n" << "maxYaw: " << maxYaw << "\n" << "minPitch: " << minPitch << "\n" << "maxPitch: " << maxPitch << "\n" << "ADS: " << ADS << "\n" << "movePen: " << movePen << "\n" << std::endl;
		if (minYaw != 0 && maxYaw != 0 && minPitch != 0 && maxPitch != 0 && ADS != 0)
		{
			printf("Recoil");

			DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
			DWORD64 recoil = mem->Read<DWORD64>(Held + oRecoil); //public RecoilProperties recoil;


			std::cout << "\n\n-----Current MinYaw: " << mem->Read<float>((recoil + oRecoilMinYaw));
			std::cout << "\n\n-----Current MaxYaw: " << mem->Read<float>((recoil + oRecoilMaxYaw));
			std::cout << "\n\n-----Current MinPitch: " << mem->Read<float>((recoil + oRecoilMinPitch));
			std::cout << "\n\n-----Current MaxPitch: " << mem->Read<float>((recoil + oRecoilMaxPitch));
			if (!mem->write<float>((recoil + oRecoilMinYaw), minYaw)) return; //public float recoilYawMin;
			if (!mem->write<float>((recoil + oRecoilMaxYaw), maxYaw)) return; //public float recoilYawMax;
			if (!mem->write<float>((recoil + oRecoilMinPitch), minPitch)) return; //public float recoilPitchMin;
			if (!mem->write<float>((recoil + oRecoilMaxPitch), maxPitch)) return; //public float recoilPitchMax;
			if (!mem->write<float>((recoil + oADSScale), ADS)) return; //public float ADSScale;
			if (!mem->write<float>((recoil + oMovementPenalty), movePen)) return; //public float movementPenalty;
			std::cout << "done...";
		}
	}

	float* ReadAimCone() {
		float* dArr = new float[2];

		DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
		dArr[0] = mem->Read<float>(Held + oAimCone);
		dArr[1] = mem->Read<float>(Held + oAimconePenaltyMax);
		return dArr;

		//if (!mem->write<float>((Held + oAimcone), movePen)) return; //public float movementPenalty;
	}

	void editAimCone(float newAimcone, float  newAimconePenaltyMax) {
		DWORD64 Held = mem->Read<DWORD64>((UINT_PTR)this + oHeld);
		if (!mem->write<float>((Held + oAimCone), newAimcone)) return;
		if (!mem->write<float>((Held + oAimconePenaltyMax), newAimconePenaltyMax)) return;
	}
};

class BasePlayer
{
public:
	//*** base entity info ***//
	/*
	Vector3 GetPosition() {
		return GetBone(GetTransform(Vars::Global::BoneToAim));
	}

	Vector3 GetBoneByID(BoneList BoneID) {
		return GetBone(GetTransform(BoneID));
	}*/



	Vector3 GetPosition() {
		DWORD64 Model = mem->Read<DWORD64>((UINT_PTR)this + oPlayerModel);
		return mem->Read<Vector3>(Model + oPlayerPosition);
	}

	int GetLookDegree() {
		DWORD64 Model = mem->Read<DWORD64>((UINT_PTR)this + oPlayerModel);
		int totalDegree = (int)mem->Read<Vector3>(Model + oSmoothLookAngle).y;
		int  actualDegree = totalDegree % 360;
		if (actualDegree < 0) {
			actualDegree = 360 + actualDegree;
		}
		return actualDegree;
	}

	ULONG GetTeam() {
		return mem->Read<ULONG>((UINT_PTR)this + oCurrentTeam);
	}

	bool isSleeping() {
		UINT64 modelFlag = mem->Read<int>(mem->Read<UINT64>((UINT_PTR)this + oModelState) + oModelFlags);
		if (modelFlag == ModelState::ModelFlags::Sleeping || modelFlag == ModelState::ModelFlags::Sleeping + ModelState::ModelFlags::OnGround)	return true;

		return false;
	}

	float GetHealth()
	{
		return mem->Read<float>((UINT_PTR)this + oPlayerHealth); //private float _health;
	}

	float GetMaxHealth()
	{
		return mem->Read<float>((UINT_PTR)this + oMaxHealth); //private float _maxHealth;
	}


	float HeliHealth() {

		return mem->Read<FLOAT>((UINT_PTR)this + 0x24); //public float health; // 0x24
	}

	const wchar_t* GetName()
	{
		pUncStr Str = ((pUncStr)(mem->Read<DWORD64>((UINT_PTR)this + oDisplayName))); //	protected string _displayName
		if (!Str) return L""; return Str->str;
	}

	/*
	Vector3 GetVelocity()
	{
		DWORD64 PlayerModel = Read<DWORD64>((UINT_PTR)this + oPlayerModel); //	public PlayerModel playerModel;
		return safe_read(PlayerModel + oVelocity, Vector3); // 	private Vector3 newVelocity;
		//обновил
	}
	*/
	/*
	bool IsVisible()
	{
		if (Vars::AimBot::VisibleCheck)
		{
			DWORD64 PlayerModel = Read<DWORD64>((UINT_PTR)this + oPlayerModel); //public PlayerModel playerModel;
			return Read<bool>(PlayerModel + oVisible); // internal bool visible;
		}
		else return true;
		//обновил
	}


	bool IsValid(bool LPlayer = false)
	{
		return (((LPlayer || Vars::PlayerEsp::IgnoreSleepers) ? !HasFlags(16) : true) && !IsDead() && (GetHealth() >= 0.8f)); GetTeamSize();
	}

	bool HasFlags(int Flg)
	{
		return (safe_read(this + oPlayerFlags, int) & Flg); //	public BasePlayer.PlayerFlags playerFlags;
		//обновил
	}

	Vector3 GetBoneByID(BoneList BoneID) {
		return GetPosition(GetTransform(BoneID));
	}

	int GetTeamSize()
	{
		DWORD64 ClientTeam = safe_read(this + oClientTeam, DWORD64); // public PlayerTeam clientTeam
		DWORD64 members = safe_read(ClientTeam + 0x30, DWORD64);//    private ListHashSet1<ILOD> members; // 0x28 ���  public List1<PlayerTeam.TeamMember> members; // 0x30
		return safe_read(members + 0x18, DWORD64);
	}

	DWORD64 IsTeamMate(int Id)
	{
		DWORD64 ClientTeam = safe_read(this + oClientTeam, DWORD64);//    public PlayerTeam clientTeam;
		DWORD64 members = safe_read(ClientTeam + 0x30, DWORD64);//    private ListHashSet1<ILOD> members; // 0x28 ���  public List1<PlayerTeam.TeamMember> members; // 0x30
		DWORD64 List = safe_read(members + 0x10, DWORD64);
		DWORD64 player = safe_read(List + 0x20 + (Id * 0x8), DWORD64); //     private BasePlayer player;
		return safe_read(player + 0x20, DWORD64);
	}

	bool IsDead()
	{
		if (!this) return true;
		return safe_read(this + oLifestate, bool); //	public BaseCombatEntity.LifeState lifestate;
	}

	DWORD64 GetSteamID()
	{
		return safe_read(this + oSteamID, DWORD64);//	public ulong userID;
	}
	*/
	DWORD64 GetSteamID()
	{
		return mem->Read<DWORD64>((UINT_PTR)this + oSteamID);//	public ulong userID;
	}
	//*** localplayer ***//

	bool IsMenu()
	{
		if (!this) return true;
		DWORD64 Input = mem->Read<DWORD64>((UINT_PTR)this + oPlayerInput); //	public PlayerInput input;
		return !(mem->Read<BOOL>(Input + oKeyFocus));// private bool hasKeyFocus;
	}

	void SetVA(const Vector2& VA)
	{
		DWORD64 Input = mem->Read<DWORD64>((UINT_PTR)this + oPlayerInput);//	public PlayerInput input;
		mem->write<Vector2>(Input + 0x3C, VA); //private Vector3 bodyAngles;
	}
	/*
	void SetRA()
	{
		if (Vars::AimBot::RCS) {
			DWORD64 Input = Read<DWORD64>((UINT_PTR)this + oPlayerInput);//	public PlayerInput input;
			Vector2 RA = { 0.f, 0.f };
			write<Vector2>(Input + 0x64, RA); //public Vector3 recoilAngles;
		}
	}
	*/
	Vector2 GetVA()
	{
		DWORD64 Input = mem->Read<DWORD64>((UINT_PTR)this + oPlayerInput);//	public PlayerInput input;
		return mem->Read<Vector2>(Input + 0x3C);//private Vector3 bodyAngles;
	}

	Vector2 GetRA()
	{
		DWORD64 Input = mem->Read<DWORD64>((UINT_PTR)this + oPlayerInput);//	public PlayerInput input;
		return mem->Read<Vector2>(Input + 0x64);//public Vector3 recoilAngles;
	}

	WeaponData* GetWeaponInfo(int Id)
	{
		DWORD64 Inventory = mem->Read<DWORD64>((UINT_PTR)this + oPlayerInventory);//	public PlayerInventory inventory;
		DWORD64 Belt = mem->Read<DWORD64>(Inventory + oContainerBelt);
		DWORD64 ItemList = mem->Read<DWORD64>(Belt + oItemList);// 	public List`1<Item> itemList;
		DWORD64 Items = mem->Read<DWORD64>(ItemList + 0x10); //	public List`1<InventoryItem.Amount> items;
		return (WeaponData*)mem->Read<DWORD64>(Items + 0x20 + (Id * 0x8));
	}

	WeaponData* GetActiveWeapon()
	{
		if (!this)
			return nullptr;

		int ActUID = mem->Read<int>((UINT_PTR)this + oActiveUID); //	private uint clActiveItem;
		//printf("\nActiveUID: %i\n", ActUID);
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
		{
			//printf("%i: %i", i, GetWeaponInfo(i)->GetUID());

			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID()) {
				//printf("\nActiveWeapon: %i\n", ActiveWeapon);
				return ActiveWeapon;
			}
		}
		return nullptr;
	}

	WeaponData* GetAitem()
	{
		if (!this)
			return nullptr;

		int ActUID = mem->Read<int>((UINT_PTR)this + oActiveUID); //	private uint clActiveItem;
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
		{
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID())
				return ActiveWeapon;
		}
		return nullptr;
	}

	//*** cheat func ***//
	/*
	void NoSway()
	{
		if (Vars::Weapon::NoSway)
		{
			write<float>((UINT_PTR)(this + oNoAimSway), 0.f);//	public float clothingAccuracyBonus;
			//РАБОТАЕТ
		}
	}
	*/
	int GetActiveItemUID()
	{
		return mem->Read<int>((UINT_PTR)this + 0x5E0);
	}

	wchar_t* GettName()
	{
		DWORD64 Info = mem->Read<DWORD64>((UINT_PTR)this + 0x20); // private string _targetName;
		DWORD64 DispName = mem->Read<DWORD64>((UINT_PTR)Info + 0x28); // public string TargetName;
		pUncStr Str = ((pUncStr)mem->Read<DWORD64>(DispName + 0x18)); // private string _TargetNamespace;
		if (!Str) return nullptr; return Str->str;
	}
};

class LPlayerBase
{
public:
	BasePlayer* BasePlayer = nullptr;
};

extern LPlayerBase LocalPlayer;


class GameData {
public:
	std::vector<BasePlayer*> Players;
	std::vector<EntityClass> Entities;
	Vector3 LocalPos;

	void updateEntities(){
		for (std::vector<EntityClass>::iterator i = Entities.begin(); i != Entities.end();)
		{
			if (i->isActive())			//If the entity has not been mined/picked
				i++;
			else
				Entities.erase(i);		//if it has been mined remove it
		}
	}

	void clear() {
		Entities.clear();
	}


};