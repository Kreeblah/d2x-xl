#ifndef _SAVEGAME_H
#define _SAVEGAME_H

#define DESC_LENGTH	20
#define NUM_SAVES		9
#define THUMBNAIL_W	100
#define THUMBNAIL_H	50
#define THUMBNAIL_LW 200
#define THUMBNAIL_LH 120


class CSaveGameManager {
	private:
		CFile			m_cf;
		int32_t		m_bInGame;
		int32_t		m_bBetweenLevels;
		int32_t		m_bSecret;
		int32_t		m_bQuick;
		int32_t		m_nDefaultSlot;
		int32_t		m_nLastSlot;
		int32_t		m_nVersion;
		int32_t		m_nGameId;
		char			m_filename [FILENAME_LEN];
		char			m_description [DESC_LENGTH + 1];
		const char*	m_override;

	public:
		CSaveGameManager () {};
		~CSaveGameManager () {};
		void Init (void);
		int32_t Save (int32_t bBetweenLevels, int32_t bSecret, int32_t bQuick, const char *pszFilenameOverride);
		int32_t Load (int32_t bInGame, int32_t bSecret, int32_t bQuick, const char *pszFilenameOverride);
		int32_t SaveState (int32_t bSecret, char *filename = NULL, char *description = NULL);
		int32_t LoadState (int32_t bMulti, int32_t bSecret, char *filename = NULL);
		int32_t GetSaveFile (int32_t bMulti);
		int32_t GetLoadFile (int32_t bMulti);
		int32_t GetGameId (char *filename, int32_t bSecret = 0);
		inline char* Filename (void) { return m_filename; }
		inline char* Description (void) { return m_description; }
		inline int32_t Version (void) { return m_nVersion; }

	private:
		void Backup (void);
		void AutoSave (int32_t nSaveSlot);
		void PushSecretSave (int32_t nSaveSlot);
		void PopSecretSave (int32_t nSaveSlot);
		void SaveImage (void);
		void SaveGameData (void);
		void SaveSpawnPoint (int32_t i);
		void SaveReactorTrigger (CTriggerTargets *pTrigger);
		void SaveReactorState (tReactorStates *pState);
		void SaveProducer (tProducerInfo *pProducer);
		void SaveObjectProducer (tObjectProducerInfo *pObjProducer);
#if 0
		void SaveObjTriggerRef (tObjTriggerRef *pRef);
#endif
		void SavePlayer (CPlayerData *pPlayer);
		void SaveNetPlayers (void);
		void SaveNetGame (void);

		int32_t ReadBoundedInt (int32_t nMax, int32_t *nVal);
		void LoadMulti (char *pszOrgCallSign, int32_t bMulti);
		int32_t LoadMission (void);
		int32_t SetServerPlayer (CPlayerData *restoredPlayers, int32_t nPlayers, const char *pszServerCallSign, int32_t *pnOtherObjNum, int32_t *pnServerObjNum);
		void GetConnectedPlayers (CPlayerData *restoredPlayers, int32_t nPlayers);
		void FixNetworkObjects (int32_t nServerPlayer, int32_t nOtherObjNum, int32_t nServerObjNum);
		void FixObjects (void);
		void AwardReturningPlayer (CPlayerData *pRetPlayer, fix xOldGameTime);;
		void LoadNetGame (void);
		void LoadNetPlayers (void);
		void LoadPlayer (CPlayerData *pPlayer);
		void LoadObjTriggerRef (tObjTriggerRef *pRef);
		void LoadObjectProducer (tObjectProducerInfo *pObjProducer);
		void LoadProducer (tProducerInfo *pProducer);
		void LoadReactorTrigger (CTriggerTargets *pTrigger);
		void LoadReactorState (tReactorStates *pState);
		int32_t LoadSpawnPoint (int32_t i);
		int32_t LoadUniFormat (int32_t bMulti, fix xOldGameTime, int32_t *nLevel);
		int32_t LoadBinFormat (int32_t bMulti, fix xOldGameTime, int32_t *nLevel);

		void SaveAILocalInfo (tAILocalInfo *pLocalInfo);
		void SaveAIPointSeg (tPointSeg *psegP);
		void SaveAICloakInfo (tAICloakInfo *ciP);
		int32_t SaveAI (void);

		int32_t LoadAIBinFormat (void);
		void LoadAILocalInfo (tAILocalInfo *pLocalInfo);
		void LoadAIPointSeg (tPointSeg *psegP);
		void LoadAICloakInfo (tAICloakInfo *ciP);
		int32_t LoadAIUniFormat (void);

		inline int32_t ImageSize (void) { return (m_nVersion < 26) ? THUMBNAIL_W * THUMBNAIL_H : THUMBNAIL_LW * THUMBNAIL_LH; }
};

extern CSaveGameManager saveGameManager;

#endif // _SAVEGAME_H
