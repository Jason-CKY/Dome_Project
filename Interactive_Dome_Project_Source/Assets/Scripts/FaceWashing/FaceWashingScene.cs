using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FaceWashingScene : MonoBehaviour
{
    private bool isActive = false;

    public GameObject guyPrefab;
    public GameObject girlPrefab;

    public List<FaceWashingSpawn> spawns = new List<FaceWashingSpawn>();

    private GameObject[] puppets;

    private float playerSeparationAngle = 30;

    public GameController gameController;

    public GameObject instructionPrefab;

    private List<InstructionPlayer> instructionSpawns = new List<InstructionPlayer>();

    private AudioSource bgm;

    void Start(){
        bgm = GetComponent<AudioSource>();
    }

    void Update()
    {
        if(isActive && gameController.gameInProgress && Input.GetKeyUp(KeyCode.P)){
            isActive = false;
            gameController.LoadPuppets(new GameObject[gameController.activePlayers.Length]);
            gameController.NextScene();
        }
    }

    public IEnumerator Init(string[] players){
        isActive = true;
        puppets = new GameObject[players.Length];

        bgm.Play();

        int currentPlayerCount = 0;

        for(int i = 0; i < players.Length; i++){
            if(players[i].Equals("1")){
                Spawn(currentPlayerCount % 2 == 0, i);
                currentPlayerCount++;
            }
        }

        int count;

        if(spawns.Count % 2 == 0){
            count = 0;
            for(int left = spawns.Count / 2 - 1; left >= 0; left--){
                spawns[left].parentTransform.rotation *= Quaternion.AngleAxis(playerSeparationAngle / 2 + count * playerSeparationAngle, Vector3.forward);
                count++;
            }
            count = 0;
            for(int right = spawns.Count / 2; right < spawns.Count; right++){
                spawns[right].parentTransform.rotation *= Quaternion.AngleAxis(playerSeparationAngle / 2 + count * playerSeparationAngle, Vector3.back);
                count++;
            }
        }
        else{
            count = 0;
            for(int index = spawns.Count / 2 - 1; index >= 0; index--){
                count++;
                spawns[index].parentTransform.rotation *= Quaternion.AngleAxis(count * playerSeparationAngle, Vector3.forward);
            }
            count = 0;
            for(int index = spawns.Count / 2 + 1; index < spawns.Count; index++){
                count++;
                spawns[index].parentTransform.rotation *= Quaternion.AngleAxis(count * playerSeparationAngle, Vector3.back);
            }
        }

        gameController.LoadPuppets(puppets);

        ShowInstructions();
        yield return new WaitForSeconds(5);
        ActivateSpawns();
    }

    public void ShowInstructions(){
        for (int i = 0; i < instructionSpawns.Count; i++) {
            StartCoroutine(instructionSpawns[i].Show(true));
        }
    }

    public void ActivateSpawns(){
        for (int i = 0; i < spawns.Count; i++) {
            spawns[i].Activate();
        }
    }

    public void DeactivateSpawns(){
        gameController.LoadPuppets(new GameObject[gameController.activePlayers.Length]);
        for (int i = 0; i < spawns.Count; i++) {
            spawns[i].Deactivate();
        }
        
        StartCoroutine(AudioFading.FadeOut(bgm, 0.5f));
    }

    private void Spawn(bool isGuy, int playerID){
        GameObject spawnObj;
        if(isGuy){
            spawnObj = Instantiate(guyPrefab, Vector3.zero + transform.position, guyPrefab.transform.rotation);
        }
        else {
            spawnObj = Instantiate(girlPrefab, Vector3.zero + transform.position, girlPrefab.transform.rotation);
        }
        spawnObj.SetActive(true);
        FaceWashingSpawn spawn = spawnObj.GetComponentInChildren<FaceWashingSpawn>();
        puppets[playerID] = spawn.GetGameObject();
        spawn.playerID = playerID;
        spawns.Add(spawn);
        spawnObj.transform.parent = transform;

        GameObject instructionObj = Instantiate(instructionPrefab, Vector3.zero, instructionPrefab.transform.rotation);
        instructionObj.transform.parent = spawnObj.transform;
        InstructionPlayer instruction = instructionObj.GetComponent<InstructionPlayer>();
        instructionSpawns.Add(instruction);
        instruction.SetPlayerId(playerID);
        instructionObj.SetActive(true);
    }

}
