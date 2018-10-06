class CustomMission: MissionServer
{	
	/* DayZ 0.63 AirDrop plugin by mov3ax / mov3ax.pro */
	/* ### ### ### ### ### ### ### ### */
	
	bool EnableAirdrops = true; // Main switch
	
	float TimesliceMultiplyier = 0.01; // Timeslice multiplyier, default value is 0.01 (60 FPS)
	float AirPlaneSpeed = 0.25; // Airplane fly speed 
	float AirPlaneHeight = 500; // Airplane fly height 
	float AirDropFallSpeed = 0.2; // Airdrop fall speed 
	// 600 Seconds = 10 Minutes
	float TicksTimerFromStart = 600 / TimesliceMultiplyier; // How much time will pass from the server start to first airdrop
	bool PrintInformationMessages = true; // Show in chat when airplane flew out and when airdrop is landed
	bool PrintInformationCoordinates = true; // Show in chat coordinates where airdrop is landed
	// 36000 Seconds = 1 Hour
	float RemoveTime = 36000 / TimesliceMultiplyier; // After how much time airplane and loot will be removed and new airplane will be spawned
	bool SpawnZombie = true; // Spawn zombie near airdrop when landed
	
	bool PrintDebugMessages = false; // Show debug messages (Debug)
	bool DropOnStart = false; // Drop airdrop instantly after airplane (Debug)
	bool TeleportDebug = false; // Teleport to airplane and airdrop during flight (Debug)
	
	bool RayReady = true; // Raycast for airdrop container fall math
	Object m_AirPlane; // Global airplane object
	Object m_AirDrop; // Glbal airdrop container object

	EntityAI m_AirDropLoot; // Airdrop container body
	ItemBase m_AirDropBase; // Airdrop container base
	Particle DropEffect; // Airdrop land particle effect	
	
	float RandomRot = 0; // Default random rotation variable
	
	// 100 Seconds
	float RandomTime = 100; // Default random drop variable

	float Delay = 0; // Local variable for security
	
	int AirDropTime = 0; // Local variable
	int AirTimer = 0; // Local variable
	int RemoveTimer = 0; // Local variable
	
	bool TimerPassed = false; // Local variable
	bool AirPassed = false; // Local variable
	bool RemovePased = false; // Local variable
	
	bool DropTime = false; // Local variable
	bool PassTime = false; // Local variable
	
	vector RandomRotOrientation; // Local variable
	
	TStringArray WorkingZombieClasses()
	{
		return {
		"ZmbM_HermitSkinny_Base","ZmbM_HermitSkinny_Beige","ZmbM_HermitSkinny_Black","ZmbM_HermitSkinny_Green",
		"ZmbM_HermitSkinny_Red","ZmbM_FarmerFat_Base","ZmbM_FarmerFat_Beige","ZmbM_FarmerFat_Blue","ZmbM_FarmerFat_Brown",
		"ZmbM_FarmerFat_Green","ZmbF_CitizenANormal_Base","ZmbF_CitizenANormal_Beige","ZmbF_CitizenANormal_Brown",
		"ZmbF_CitizenANormal_Blue","ZmbM_CitizenASkinny_Base","ZmbM_CitizenASkinny_Blue","ZmbM_CitizenASkinny_Brown",
		"ZmbM_CitizenASkinny_Grey","ZmbM_CitizenASkinny_Red","ZmbM_CitizenBFat_Base","ZmbM_CitizenBFat_Blue","ZmbM_CitizenBFat_Red",
		"ZmbM_CitizenBFat_Green","ZmbF_CitizenBSkinny_Base","ZmbF_CitizenBSkinny","ZmbM_PrisonerSkinny_Base","ZmbM_PrisonerSkinny",
		"ZmbM_FirefighterNormal_Base","ZmbM_FirefighterNormal","ZmbM_FishermanOld_Base","ZmbM_FishermanOld_Blue","ZmbM_FishermanOld_Green",
		"ZmbM_FishermanOld_Grey","ZmbM_FishermanOld_Red","ZmbM_JournalistSkinny_Base","ZmbM_JournalistSkinny","ZmbF_JournalistNormal_Base",
		"ZmbF_JournalistNormal_Blue","ZmbF_JournalistNormal_Green","ZmbF_JournalistNormal_Red","ZmbF_JournalistNormal_White",
		"ZmbM_ParamedicNormal_Base","ZmbM_ParamedicNormal_Blue","ZmbM_ParamedicNormal_Green","ZmbM_ParamedicNormal_Red",
		"ZmbM_ParamedicNormal_Black","ZmbF_ParamedicNormal_Base","ZmbF_ParamedicNormal_Blue","ZmbF_ParamedicNormal_Green",
		"ZmbF_ParamedicNormal_Red","ZmbM_HikerSkinny_Base","ZmbM_HikerSkinny_Blue","ZmbM_HikerSkinny_Green","ZmbM_HikerSkinny_Yellow",
		"ZmbF_HikerSkinny_Base","ZmbF_HikerSkinny_Blue","ZmbF_HikerSkinny_Grey","ZmbF_HikerSkinny_Green","ZmbF_HikerSkinny_Red",
		"ZmbM_HunterOld_Base","ZmbM_HunterOld_Autumn","ZmbM_HunterOld_Spring","ZmbM_HunterOld_Summer","ZmbM_HunterOld_Winter",
		"ZmbF_SurvivorNormal_Base","ZmbF_SurvivorNormal_Blue","ZmbF_SurvivorNormal_Orange","ZmbF_SurvivorNormal_Red",
		"ZmbF_SurvivorNormal_White","ZmbM_SurvivorDean_Base","ZmbM_SurvivorDean_Black","ZmbM_SurvivorDean_Blue","ZmbM_SurvivorDean_Grey",
		"ZmbM_PolicemanFat_Base","ZmbM_PolicemanFat","ZmbF_PoliceWomanNormal_Base","ZmbF_PoliceWomanNormal","ZmbM_PolicemanSpecForce_Base",
		"ZmbM_PolicemanSpecForce","ZmbM_SoldierNormal_Base","ZmbM_SoldierNormal","ZmbM_usSoldier_normal_Base",
		"ZmbM_usSoldier_normal_Woodland","ZmbM_usSoldier_normal_Desert","ZmbM_CommercialPilotOld_Base","ZmbM_CommercialPilotOld_Blue",
		"ZmbM_CommercialPilotOld_Olive","ZmbM_CommercialPilotOld_Brown","ZmbM_CommercialPilotOld_Grey","ZmbM_PatrolNormal_Base",
		"ZmbM_PatrolNormal_PautRev","ZmbM_PatrolNormal_Autumn","ZmbM_PatrolNormal_Flat","ZmbM_PatrolNormal_Summer","ZmbM_JoggerSkinny_Base",
		"ZmbM_JoggerSkinny_Blue","ZmbM_JoggerSkinny_Green","ZmbM_JoggerSkinny_Red","ZmbF_JoggerSkinny_Base","ZmbF_JoggerSkinny_Blue",
		"ZmbF_JoggerSkinny_Brown","ZmbF_JoggerSkinny_Green","ZmbF_JoggerSkinny_Red","ZmbM_MotobikerFat_Base","ZmbM_MotobikerFat_Beige",
		"ZmbM_MotobikerFat_Black","ZmbM_MotobikerFat_Blue","ZmbM_VillagerOld_Base","ZmbM_VillagerOld_Blue","ZmbM_VillagerOld_Green",
		"ZmbM_VillagerOld_White","ZmbM_SkaterYoung_Base","ZmbM_SkaterYoung_Blue","ZmbM_SkaterYoung_Brown","ZmbM_SkaterYoung_Green",
		"ZmbM_SkaterYoung_Grey","ZmbF_SkaterYoung_Base","ZmbF_SkaterYoung_Brown","ZmbF_SkaterYoung_Striped","ZmbF_SkaterYoung_Violet",
		"ZmbF_DoctorSkinny_Base","ZmbF_DoctorSkinny","ZmbF_BlueCollarFat_Base","ZmbF_BlueCollarFat_Blue","ZmbF_BlueCollarFat_Green",
		"ZmbF_BlueCollarFat_Red","ZmbF_BlueCollarFat_White","ZmbF_MechanicNormal_Base","ZmbF_MechanicNormal_Beige","ZmbF_MechanicNormal_Green",
		"ZmbF_MechanicNormal_Grey","ZmbF_MechanicNormal_Orange","ZmbM_MechanicSkinny_Base","ZmbM_MechanicSkinny_Blue","ZmbM_MechanicSkinny_Grey",
		"ZmbM_MechanicSkinny_Green","ZmbM_MechanicSkinny_Red","ZmbM_ConstrWorkerNormal_Base","ZmbM_ConstrWorkerNormal_Beige",
		"ZmbM_ConstrWorkerNormal_Black","ZmbM_ConstrWorkerNormal_Green","ZmbM_ConstrWorkerNormal_Grey","ZmbM_HeavyIndustryWorker_Base",
		"ZmbM_HeavyIndustryWorker","ZmbM_OffshoreWorker_Base","ZmbM_OffshoreWorker_Green","ZmbM_OffshoreWorker_Orange","ZmbM_OffshoreWorker_Red",
		"ZmbM_OffshoreWorker_Yellow","ZmbF_NurseFat_Base","ZmbF_NurseFat","ZmbM_HandymanNormal_Base","ZmbM_HandymanNormal_Beige",
		"ZmbM_HandymanNormal_Blue","ZmbM_HandymanNormal_Green","ZmbM_HandymanNormal_Grey","ZmbM_HandymanNormal_White","ZmbM_DoctorFat_Base",
		"ZmbM_DoctorFat","ZmbM_Jacket_Base","ZmbM_Jacket_beige","ZmbM_Jacket_black","ZmbM_Jacket_blue","ZmbM_Jacket_bluechecks",
		"ZmbM_Jacket_brown","ZmbM_Jacket_greenchecks","ZmbM_Jacket_grey","ZmbM_Jacket_khaki","ZmbM_Jacket_magenta","ZmbM_Jacket_stripes",
		"ZmbF_PatientOld_Base","ZmbF_PatientOld","ZmbM_PatientSkinny_Base","ZmbM_PatientSkinny","ZmbF_ShortSkirt_Base","ZmbF_ShortSkirt_beige",
		"ZmbF_ShortSkirt_black","ZmbF_ShortSkirt_brown","ZmbF_ShortSkirt_green","ZmbF_ShortSkirt_grey","ZmbF_ShortSkirt_checks",
		"ZmbF_ShortSkirt_red","ZmbF_ShortSkirt_stripes","ZmbF_ShortSkirt_white","ZmbF_ShortSkirt_yellow","ZmbF_VillagerOld_Base",
		"ZmbF_VillagerOld_Blue","ZmbF_VillagerOld_Green","ZmbF_VillagerOld_Red","ZmbF_VillagerOld_White","ZmbM_Soldier","ZmbM_SoldierAlice",
		"ZmbM_SoldierHelmet","ZmbM_SoldierVest","ZmbM_SoldierAliceHelmet","ZmbM_SoldierVestHelmet","ZmbF_MilkMaidOld_Base",
		"ZmbF_MilkMaidOld_Beige","ZmbF_MilkMaidOld_Black","ZmbF_MilkMaidOld_Green","ZmbF_MilkMaidOld_Grey","ZmbM_priestPopSkinny_Base",
		"ZmbM_priestPopSkinny","ZmbM_ClerkFat_Base","ZmbM_ClerkFat_Brown","ZmbM_ClerkFat_Grey","ZmbM_ClerkFat_Khaki","ZmbM_ClerkFat_White",
		"ZmbF_Clerk_Normal_Base","ZmbF_Clerk_Normal_Blue","ZmbF_Clerk_Normal_White","ZmbF_Clerk_Normal_Green","ZmbF_Clerk_Normal_Red",
		};
	}
	
	void SendMessage(string message) // Send message in global chat
	{
		ref array<Man> players = new array<Man>; 
		GetGame().GetPlayers( players ); 
		
		for ( int i = 0; i < players.Count(); i++ ) 
		{ 
			PlayerBase player1; 
			Class.CastTo(player1, players.Get(i)); 

			Param1<string> m_AirTimer = new Param1<string>(message); 
			GetGame().RPCSingleParam(player1, ERPCs.RPC_USER_ACTION_MESSAGE, m_AirTimer, true, player1.GetIdentity()); 
		}
	}
	
	void SendPos(vector pos) // Teleport all players to position, debug
	{
		ref array<Man> players = new array<Man>; 
		GetGame().GetPlayers( players ); 
		
		for ( int i = 0; i < players.Count(); i++ ) 
		{ 
			PlayerBase player1; 
			Class.CastTo(player1, players.Get(i)); 

			player1.SetPosition(pos);
		}
	}

	string GetRandomLoot() // Random Loot Presets
	{
		string loot;
		switch (Math.RandomInt(0, 6)) {
		case 0:
		loot = "LandMineTrap";
		break;
		case 1:
		loot = "TTSKOPants";
		break;
		case 2:
		loot = "TacticalBaconCan";
		break;
		case 3:
		loot = "M4A1";
		break;
		case 4:
		loot = "PlateCarrierComplete";
		break;
		case 5:
		loot = "BakedBeansCan";
		break;
		case 6:
		loot = "WaterBottle";
		break;
		}
		
		return loot;
	}
	
	// Generating random airdrop position from list
	// You can get coordinates using debug monitor or this map https://dayz.ginfo.gg/
	vector GetAirPlanePos() 
	{
		protected vector airplane_yaw; // Local variable
		airplane_yaw[1] = AirPlaneHeight; // Airplane fly height 
		
		switch (Math.RandomInt(0, 2)) {
		case 0:
		airplane_yaw[0] = 5500;
		airplane_yaw[2] = 500;
		break;
		case 1:
		airplane_yaw[0] = 2700;
		airplane_yaw[2] = 700;
		break;
		case 2:
		airplane_yaw[0] = 10000;
		airplane_yaw[2] = 1000;
		break;
		}
		
		return airplane_yaw;
	}

	void SpawnAirPlaneAndTeleportPlayer_DEBUG()
	{
		// Seconds devide on value of TimesliceMultiplyier (By default it is 0.01)
		RandomTime = Math.RandomFloat(95 / TimesliceMultiplyier, 180 / TimesliceMultiplyier); // Random drop bounds
		RandomRot = Math.RandomFloat(130, 190); // Random rot bounds

		if (PrintDebugMessages)
			SendMessage("Debug - RandomRot " + RandomRot);		
		
        // Dynamic movement forward
        float rotationYaw = RandomRot;
        float f = rotationYaw * 0.017453292;
		float motionX  = (double)(Math.Sin(f) * 7.5); // 7.5 is airdrop container motion speed
        float motionZ = (double)(Math.Cos(f) * 7.5); // 7.5 is airdrop container motion speed

		// Fixed position, if we dont multiply value to -1 it will move backwards
        protected vector m_AirPlaneFixedPosition;
		m_AirPlaneFixedPosition[0] = motionX * -1;
		m_AirPlaneFixedPosition[1] = 10;
		m_AirPlaneFixedPosition[2] = motionZ * -1;
	
		RandomRotOrientation[0] = RandomRot;
		
        m_AirPlane = GetGame().CreateObject( "Land_Wreck_C130J", GetAirPlanePos(), false, true ); // Create airplane model
		m_AirPlane.SetOrientation(RandomRotOrientation); // Rotate it to random angles in yaw

		//FIXME: Broken sound raduis
        GetGame().CreateSoundOnObject(m_AirPlane, "powerGeneratorLoop", 10000, true, false); // Attach airplane sound to itself
		
		if (PrintInformationMessages)
			SendMessage("Airplane flew out!");		

		protected vector m_AirPlanePos = m_AirPlane.GetPosition();		
        protected vector m_NewAirPlanePos = m_AirPlanePos - "0 0 0";	

        m_AirPlane.SetPosition(m_NewAirPlanePos);

        protected vector m_AirPlanePosDYN = m_AirPlane.GetPosition();
		protected vector m_NewAirPlanePosDYN = (m_AirPlanePos) - m_AirPlaneFixedPosition;

        protected vector m_DropPos = m_NewAirPlanePosDYN;	
				
		if (DropOnStart) // For debug puproses only
		{					
			RayReady = false;
			m_AirDrop = GetGame().CreateObject( "Land_Container_1Bo", m_DropPos, false, true ); // Create airdrop model, in this case it is red container
			
			if (PrintInformationMessages)
				SendMessage("Airdrop dropped!");	
			
			// Reset it to default values
			RemoveTimer = 0;
			RemovePased = false;
		}

		if (PrintDebugMessages)
		{
			if (PrintInformationMessages)
				SendMessage("Airdrop dropped!");
			
			if (TeleportDebug)
				SendPos(m_AirPlane.GetPosition());
		}			
    }

    void SpawnAirPlaneAndMove_DEBUG()
	{	
        // Dynamic movement forward
		float rotationYaw = RandomRot;
        float f = rotationYaw * 0.017453292;
		float motionX  = (double)(Math.Sin(f) * AirPlaneSpeed); // Airplane speed
        float motionZ = (double)(Math.Cos(f) * AirPlaneSpeed); // Airplane speed

		// Fixed position, if we dont multiply value to -1 it will move backwards
        protected vector m_AirPlaneFixedPosition;
		m_AirPlaneFixedPosition[0] = motionX * -1;
		m_AirPlaneFixedPosition[1] = 0;
		m_AirPlaneFixedPosition[2] = motionZ * -1;

        protected vector m_AirPlanePos = m_AirPlane.GetPosition();
		protected vector m_NewAirPlanePos = m_AirPlanePos + m_AirPlaneFixedPosition;

        m_AirPlane.SetPosition(m_NewAirPlanePos);

		protected vector m_AirDropFallSpeed;
		m_AirDropFallSpeed[0] = 0;
		m_AirDropFallSpeed[1] = AirDropFallSpeed;
		m_AirDropFallSpeed[2] = 0;
		
        protected vector m_OldAirDropPos;
	protected vector m_NewAirDropPos;

	if (m_AirDrop != NULL)
        	m_OldAirDropPos = m_AirDrop.GetPosition();
	else
		m_OldAirDropPos = "0 0 0";

	m_NewAirDropPos = m_OldAirDropPos - m_AirDropFallSpeed;

        // Raycast, check if airdrop is on ground
        vector rayStart;
	if (m_AirDrop != NULL)
		rayStart = m_AirDrop.GetPosition() - "0 1.1 0";
	else
		rayStart = "0 0 0";
		
	vector rayEnd = m_NewAirDropPos;
	vector hitPos;
	vector hitNormal;
	int hitComponentIndex;
		
        protected vector m_AirPlanePosDYN = m_AirPlane.GetPosition();
	protected vector m_NewAirPlanePosDYN = m_AirPlanePos - "0 10 0";

        protected vector m_DropPos = m_NewAirPlanePosDYN;	
			
		if (!RayReady)
		{
			if (DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, m_AirDrop))
			{
				m_AirDrop.SetPosition(hitPos + "0 1.1 0");
				m_AirDrop.SetOrientation(hitNormal);
				
				// Create airdrop lootable container, in this case it is sea chest	
				m_AirDropLoot = EntityAI.Cast(GetGame().CreateObject( "SeaChest", m_AirDrop.GetPosition(), false, true )); // We can't add barrel because it have to be opened
				
				// You can extend items count inside airdrop container or make it random
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				m_AirDropLoot.GetInventory().CreateInInventory(GetRandomLoot());
				
				m_AirDropBase = ItemBase.Cast(m_AirDropLoot); // Cast items to airdrop container

				// Play particle when airdrop container, in this case it is red container
				DropEffect = Particle.Play( ParticleList.EXPLOSION_LANDMINE, m_AirDrop.GetPosition() );
			
				if (PrintInformationMessages && !PrintInformationCoordinates)
					SendMessage("Airdrop landed!");		
				else if (PrintInformationMessages && PrintInformationCoordinates)
					SendMessage("Airdrop landed at " + m_AirDrop.GetPosition() + "!");		
					
				if (SpawnZombie)
				{
					GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), m_AirDrop.GetPosition() - "10 0 0", false, true );	
					GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), m_AirDrop.GetPosition() - "0 0 10", false, true );	
					GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), m_AirDrop.GetPosition() - "10 0 -10", false, true );		
					GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), m_AirDrop.GetPosition() - "-10 0 10", false, true );
				}
				
				// Reset it to default values
				RayReady = true;
			}
			else
			{
				m_AirDrop.SetPosition(m_NewAirDropPos);
			}
		}
		
		if (DropTime && !DropOnStart)
		{			
			RayReady = false;	
			
			m_AirDrop = GetGame().CreateObject( "Land_Container_1Bo", m_DropPos, false, true ); // Create airdrop model, in this case it is red container

			// Reset it to default values
			RemoveTimer = 0;
			RemovePased = false;
			DropTime = false;		
		}
    }	
			
	void CreateAirDrop()
	{
		// Remove timer		
		if (RemoveTimer <= RemoveTime)
		{
			RemoveTimer++;
		}
		else
		{
			// Removing all objects when remove timer is passed
			if (!RemovePased)
			{
				if (m_AirDrop != NULL)
				{
					m_AirDrop.SetPosition(vector.Zero);
					GetGame().ObjectDelete( m_AirDrop ); 
					m_AirDrop = NULL;	
				}
				
				if (m_AirPlane != NULL)
				{
					m_AirPlane.SetPosition(vector.Zero);
					GetGame().ObjectDelete( m_AirPlane ); 
					m_AirPlane = NULL;	
				}
				
				if (m_AirDropBase != NULL)
				{
					m_AirDropBase.SetPosition(vector.Zero);
					GetGame().ObjectDelete( m_AirDropBase ); 
					m_AirDropBase = NULL;	
				}
				
				if (m_AirDropLoot != NULL)
				{
					m_AirDropLoot.SetPosition(vector.Zero);
					GetGame().ObjectDelete( m_AirDropLoot ); 
					m_AirDropLoot = NULL;	
				}
				
				// Reset it to default values
				AirPassed = false;
				TimerPassed = false;
				AirDropTime = 0;
				AirTimer = 0;	
				DropTime = false;		
				RemovePased = true;	
			}
		}
		
		//  After how much time after restart need to wait before airplane spawn
		if (AirTimer <= TicksTimerFromStart)
		{
			AirTimer++;
			
			if (PrintDebugMessages)
				SendMessage("Debug - " + AirTimer);		
		}
		else
		{	
			// Reset it to default values
			TimerPassed = true;
		}

		if (TimerPassed && !AirPassed)	
		{
			if (PrintDebugMessages)
				SendMessage("Debug - Ticks Passed!");		
			
			SpawnAirPlaneAndTeleportPlayer_DEBUG();
			
			if (PrintDebugMessages)
				SendMessage("Debug - Test! Spawned via timer");		
				
			// Reset it to default values
			AirPassed = true;
		}	

		if (AirPassed)
		{
			if (m_AirPlane)
			{
				SpawnAirPlaneAndMove_DEBUG();
			}
		}
			
		if (AirDropTime <= RandomTime)
		{
			AirDropTime++;
			
			if (PrintDebugMessages)
			{
				if (TeleportDebug)
					SendPos(m_AirPlane.GetPosition());
				
				SendMessage("Debug Time - " + AirDropTime);		
				SendMessage("Random Time - " + RandomTime);		
			}
		}
		else
		{
			if (Delay <= 1000)
			{
				Delay++;
			}
			else
			{
				if (!RayReady)
				{
					if (TeleportDebug)
						SendPos(m_AirDrop.GetPosition());
				}
			
				if (!PassTime)
				{
					// Reset it to default values
					DropTime = true;
					PassTime = true;
				}
			}
		}
	}
	
	/* ### ### ### ### ### ### ### ### */
	
	float TimerSlice;
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		// FPS Fix
		TimerSlice += timeslice;
		if (TimerSlice >= TimesliceMultiplyier)
		{
			if (EnableAirdrops)
				CreateAirDrop();
			TimerSlice = 0;	
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
