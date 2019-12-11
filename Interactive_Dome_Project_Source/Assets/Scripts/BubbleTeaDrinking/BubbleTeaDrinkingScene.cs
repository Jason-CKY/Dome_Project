using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BubbleTeaDrinkingScene : MonoBehaviour
{
    private bool isActive = false;
    public GameObject bubbleTeaPrefab;

    public List<BubbleTeaDrinkingSpawn> spawns = new List<BubbleTeaDrinkingSpawn>();
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

        for(int i = 0; i < players.Length; i++){
            if(players[i].Equals("1")){
                Spawn(i % 2 == 0, i);
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
        isActive = false;
        gameController.LoadPuppets(new GameObject[gameController.activePlayers.Length]);
        for (int i = 0; i < spawns.Count; i++) {
            spawns[i].Deactivate();
        }
   
        StartCoroutine(AudioFading.FadeOut(bgm, 0.5f));
    }

    private void Spawn(bool isGuy, int playerID){
        GameObject spawnObj = Instantiate(bubbleTeaPrefab, Vector3.zero + transform.position, bubbleTeaPrefab.transform.rotation);
        spawnObj.SetActive(true);
        BubbleTeaDrinkingSpawn spawn = spawnObj.GetComponentInChildren<BubbleTeaDrinkingSpawn>();
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
