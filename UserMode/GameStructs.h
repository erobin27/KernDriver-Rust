#pragma once
#include "UM-Memory.h"
#include "settings.h"

//game structs
#define g_BN 0x2985810 
#define g_BN_Steam 0x2985810
#define oPlayerModel 0x4A8 //public PlayerModel playerModel;
#define oPlayerFlags 0x5F8 //	public BasePlayer.PlayerFlags playerFlags;
#define oHeld 0x98  // private EntityRef heldEntity;
#define oPlayerInventory 0x608  //	public PlayerInventory inventory;
#define oPlayerInput 0x4C8  //	public PlayerInput input;
#define oPlayerMovement 0x4D0  // public BaseMovement movement;
#define oSteamID 0x640 //	public ulong userID;
#define oPlayerName 0x650    //	protected string _displayName
#define oNoBlockAiming 0x6B0 //public bool clothingBlocksAiming;
#define oSpeedReduction 0x6B4 //	public float clothingMoveSpeedReduction;
#define oMagazine 0x2A0 
#define oVisible 0x248  // internal bool visible;
#define oNoAimSway 0x6BC     //	public float clothingAccuracyBonus;
#define oLifestate 0x204 //	public BaseCombatEntity.LifeState lifestate;
#define oClientTeam 0x540 // public PlayerTeam clientTeam
#define oActiveUID 0x570  //	private uint clActiveItem;
#define oPlayerHealth 0x20C  //private float _health;
#define oAuto 0x270   // public bool automatic;
#define oDistance 0x278 //private Transform attachmentBoneCache; public float maxDistance; // 0x278
#define oAttackRadius 0x27C //public float attackRadius;
#define oVelocity 0x1FC  // 	private Vector3 newVelocity;
#define oFakeAdmin 0x5F8   //public BasePlayer.PlayerFlags playerFlags;
#define oSuccessFraction 0x340  //public float successFraction;
#define oGroundAngle 0xb4 // private float groundAngle;
#define oGroundAngleNew 0xB8  // private float groundAngleNew;
#define oGravity 0x74  //public float gravityMultiplier;
#define oWaterBoost 0x6B8   //	public float clothingWaterSpeedBonus;
#define oFlyHack  0x13C     // private bool flying;
#define oMultiplierSwimming 0x78  // public float gravityMultiplierSwimming;
#define oTickTime 0x5CC //	private float lastSentTickTime;
#define oFrozen 0x4B0 // public bool Frozen;
#define oKeyFocus 0x94  // private bool hasKeyFocus;
#define oClientTickInterval 0x5C8  // public float clientTickInterval;
//#define oNoClip  0x563900     //public static void noclip(ConsoleSystem.Arg arg) { }
//Recoil
#define oRecoilMinYaw 0x18 //public float recoilYawMin
#define oRecoilMaxYaw 0x1C
#define oRecoilMinPitch 0x20
#define oRecoilMaxPitch 0x24
#define oADSScale 0x30 //public float ADSScale;
#define oMovementPenalty 0x34 //public float movementPenalty;
#define oRecoil 0x2C0  //public RecoilProperties recoil;
//AntiSpread
#define oAimconePenaltyPerShot 0x2D8  // public float aimconePenaltyPerShot;
#define oAimCone 0x2D0  // public float aimCone;
#define oHipAimCone 0x2D4 // public float hipAimCone;
#define oStancePenalty 0x304 // private float stancePenalty;
#define oAimConePenalty 0x308 // private float aimconePenalty;

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

typedef struct _UncStr
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;

class WeaponData
{
public:
	wchar_t* GetName()
	{
		DWORD64 Info = Read<DWORD64>((UINT_PTR)this + 0x20); // private string _targetName;
		DWORD64 DispName = Read<DWORD64>(Info + 0x28); // public string TargetName;
		pUncStr Str = ((pUncStr)Read<DWORD64>(DispName + 0x18)); // private string _TargetNamespace;
		if (!Str) return nullptr; return Str->str;
	}
	int GetID() {
		DWORD64 Info = Read<DWORD64>((DWORD64)this + 0x20);
		int ID = Read<int>(Info + 0x18);
		return ID;
	}
	void NoRecoil()
	{
		if (Vars::Weapon::NoRecoil)
		{
			DWORD64 Held = Read<DWORD64>((UINT_PTR)this + oHeld);
			DWORD64 recoil = Read<DWORD64>(Held + oRecoil); //public RecoilProperties recoil; ?? ?????????? ???????.
			write<float>(recoil + oRecoilMinYaw, Vars::Weapon::Recoil); //public float recoilYawMin; ?? ?????????? ???????.
			write<float>(recoil + oRecoilMaxYaw, Vars::Weapon::Recoil); //public float recoilYawMax; ?? ?????????? ???????.
			write<float>(recoil + oRecoilMinPitch, Vars::Weapon::Recoil); //public float recoilPitchMin; ?? ?????????? ???????.
			write<float>(recoil + oRecoilMaxPitch, Vars::Weapon::Recoil); //public float recoilPitchMax; ?? ?????????? ???????.
			write<float>(recoil + oADSScale, Vars::Weapon::Recoil); //public float ADSScale; ?? ?????????? ???????.
			write<float>(recoil + oMovementPenalty, Vars::Weapon::Recoil); //public float movementPenalty; ?? ?????????? ???????.
	//????????
		}
	}

	void AntiSpread()
	{
		if (Vars::Weapon::AntiSpread)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
			safe_write(Held + oStancePenalty, 0.f, float); // private float stancePenalty;
			safe_write(Held + oAimConePenalty, 0.f, float); // private float aimconePenalty;
			safe_write(Held + oAimCone, 0.f, float); // public float aimCone;
			safe_write(Held + oHipAimCone, 0.f, float); // public float hipAimCone;
			safe_write(Held + oAimconePenaltyPerShot, 0.f, float); // public float aimconePenaltyPerShot;
		}
	}