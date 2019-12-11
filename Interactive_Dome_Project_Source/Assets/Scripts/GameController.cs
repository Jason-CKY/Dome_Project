using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameController : MonoBehaviour
{
    public int sceneID;

    public ArduinoReader arduino;
    public GameObject mainCamera;

    public GameObject bgCylinder;
    public Material bgMaterial;

    public Texture2D[] bgTextures;
    
    public GameObject videoCamera;
    public GameObject videoPlane;

    public VideoController videoController;
    public Material videoMaterial;

    public Texture2D videoBlackTex;
    public Texture2D videoWhiteTex;
    public Material coverMaterial;

    public FaceWashingScene faceWashingScene;
    public BubbleTeaDrinkingScene bubbleTeaDrinkingScene;
    public HazeFanningScene hazeFanningScene;

    public string[] activePlayers;

    private int activePlayerCount = 4;
    public GameObject[] puppets;

    private FaceWashingSpawn[] faceWashingSpawns;
    private BubbleTeaDrinkingSpawn[] bubbleTeaDrinkingSpawns;
    private HazeFanningSpawn[] hazeFanningSpawns;
    
    public GameObject EndTextureObj;

    public int[] scores = new int[8];
    private int[] ranks;
    private int currentRank = 1;

    public Sprite[] rankSprites;

    public bool gameInProgress = false;

    private bool detectPlayers;

    public Light pointLight;

    public GameObject rotatingControllerRoot;

    public ControllerRotater[] rotatingControllers;

    public Winner winner;

    private AudioSource bgm;

    private bool firstSample = true;

    void Start(){
        bgm = GetComponent<AudioSource>();
        if(rotatingControllerRoot.activeSelf){
            sceneID = -2;
            coverMaterial.color = new Color(0, 0, 0, 0);
            bgMaterial.mainTexture = bgTextures[0];
            bgm.Play();
            for(int i = 0; i < rotatingControllers.Length; i++){
                rotatingControllers[i].Show();
            }

            detectPlayers = true;
        }
        else{
            coverMaterial.color = Color.black;
        }
        NextScene();
    }

    void Update(){
        if(Input.GetKeyUp(KeyCode.Escape)){
            arduino.Stop();
            Application.Quit();
        }
        else if(Input.GetKeyUp(KeyCode.F1)){
            arduino.Stop();
            SceneManager.LoadScene("Final");
        }
        else if(rotatingControllerRoot.activeSelf && Input.GetKeyUp(KeyCode.P) && sceneID == -1){
            NextScene();
        }


        if(!gameInProgress) return;

        if(puppets[0] != null){
            if(Input.GetKeyUp(KeyCode.Alpha1)){
                puppets[0].SendMessage("Animate",1.0f);
            }
            if(Input.GetKeyUp(KeyCode.Alpha2)){
                puppets[0].SendMessage("Animate",2.0f);
            }
            if(Input.GetKeyUp(KeyCode.Alpha3)){
                puppets[0].SendMessage("Animate",3.0f);
            }
            if(Input.GetKeyUp(KeyCode.Alpha4)){
                puppets[0].SendMessage("Animate",-1f);
            }
        }

        if(puppets[1] != null){
            if(Input.GetKeyUp(KeyCode.Q)){
                puppets[1].SendMessage("Animate",1.0f);
            }
            if(Input.GetKeyUp(KeyCode.W)){
                puppets[1].SendMessage("Animate",2.0f);
            }
            if(Input.GetKeyUp(KeyCode.E)){
                puppets[1].SendMessage("Animate",3.0f);
            }
            if(Input.GetKeyUp(KeyCode.R)){
                puppets[1].SendMessage("Animate",-1f);
            }
        }

        if(puppets[2] != null){
            if(Input.GetKeyUp(KeyCode.A)){
                puppets[2].SendMessage("Animate",1.0f);
            }
            if(Input.GetKeyUp(KeyCode.S)){
                puppets[2].SendMessage("Animate",2.0f);
            }
            if(Input.GetKeyUp(KeyCode.D)){
                puppets[2].SendMessage("Animate",3.0f);
            }
            if(Input.GetKeyUp(KeyCode.F)){
                puppets[2].SendMessage("Animate",-1f);
            }
        }

        if(puppets[3] != null){
            if(Input.GetKeyUp(KeyCode.Z)){
                puppets[3].SendMessage("Animate",1.0f);
            }
            if(Input.GetKeyUp(KeyCode.X)){
                puppets[3].SendMessage("Animate",2.0f);
            }
            if(Input.GetKeyUp(KeyCode.C)){
                puppets[3].SendMessage("Animate",3.0f);
            }
            if(Input.GetKeyUp(KeyCode.V)){
                puppets[3].SendMessage("Animate",-1f);
            }
        }
        
    }

    IEnumerator ShowCover(Color targetColour)
    {
        if(targetColour == Color.white){
            videoMaterial.mainTexture = videoWhiteTex;
        }
        else{
            videoMaterial.mainTexture = videoBlackTex;
        }
        float progress = 0;
        float smoothness = 0.02f;
        float duration = 1f;
        float increment = smoothness/duration;
        float alpha = 0;
        while(progress < 1)
        {
            alpha = Mathf.Lerp(0.0f, 1.0f, progress) ;
            coverMaterial.color = new Color(targetColour.r, targetColour.g, targetColour.b, alpha);
            progress += increment;
            yield return new WaitForSeconds(smoothness);
        }
    }

    IEnumerator HideCover(Color targetColour)
    {
        if(targetColour == Color.white){
            videoMaterial.mainTexture = videoWhiteTex;
        }
        else{
            videoMaterial.mainTexture = videoBlackTex;
        }
        float progress = 0;
        float smoothness = 0.02f;
        float duration = 1f;
        float increment = smoothness/duration;
        float alpha = 0;
        
        while(progress < 1)
        {
            alpha = Mathf.Lerp(1.0f, 0f, progress) ;
            coverMaterial.color = new Color(targetColour.r,targetColour.g, targetColour.b, alpha);
            progress += increment;
            yield return new WaitForSeconds(smoothness);
        }
    }

    IEnumerator playTransitionVideo(Color colour, int clipIndex){
        yield return StartCoroutine(ShowCover(colour));
        videoCamera.SetActive(true);
        mainCamera.SetActive(false);
        videoPlane.SetActive(true);
        videoController.playVideo(clipIndex);
    }

    public void NextScene(){
        gameInProgress = false;
        
        iTween.Stop(bgCylinder);
        
        for(int i = 0; i < transform.childCount; i++){
            Transform child = transform.GetChild(i);
            if(child.gameObject.activeInHierarchy){
                child.SendMessage("Vanish");
            }
        }
        switch(sceneID){
            case -2:
                iTween.RotateBy (bgCylinder, iTween.Hash("amount", new Vector3(0, 0.08f, 0), "time", 3, "easetype", iTween.EaseType.easeInOutSine, "looptype", iTween.LoopType.pingPong));
            break;
            case -1:
            for(int i = 0; i < rotatingControllers.Length; i++){
                rotatingControllers[i].Hide();
            }
            StartCoroutine(playTransitionVideo(Color.black, 0));
            StartCoroutine(AudioFading.FadeOut(bgm, 0.5f));
            break;
            case 0:
            StartCoroutine(LoadFaceWashing());
            break;
            case 1:
            faceWashingScene.DeactivateSpawns();
            StartCoroutine(playTransitionVideo(Color.black, 1));
            break;
            case 2:
            StartCoroutine(LoadBubbleTeaDrinking());
            break;
            case 3:
            bubbleTeaDrinkingScene.DeactivateSpawns();
            StartCoroutine(playTransitionVideo(Color.black, 2));
            break;
            case 4:
            StartCoroutine(LoadHazeFanning());
            break;
            case 5:
            hazeFanningScene.DeactivateSpawns();
            StartCoroutine(playTransitionVideo(Color.white, 3));
            break;
            case 6:
            StartCoroutine(LoadWinner());
            break;
            case 7:
            StartCoroutine(playTransitionVideo(Color.black, 4));
            iTween.RotateBy (videoPlane, iTween.Hash("z", 1f, "time", 120, "easetype", iTween.EaseType.linear, "looptype", iTween.LoopType.loop));
            break;
        }
        sceneID++;
    }

    IEnumerator UnloadHazeScene()
    {
        hazeFanningScene.DeactivateSpawns();
        yield return new WaitForSeconds(5);
        yield return StartCoroutine(playTransitionVideo(Color.white, 3));
    }

    IEnumerator LoadFaceWashing()
    {
        videoPlane.SetActive(false);
        mainCamera.SetActive(true);
        videoCamera.SetActive(false);
        rotatingControllerRoot.SetActive(false);
        bgMaterial.mainTexture = bgTextures[1];
        //bgCylinder.transform.rotation *= Quaternion.AngleAxis(83, Vector3.up);
        bgCylinder.transform.rotation = Quaternion.Euler(0, 353, 0);
        iTween.RotateTo(bgCylinder, iTween.Hash("rotation", new Vector3(0, 83, 0), "time", 8, "easetype", iTween.EaseType.easeOutSine));
        coverMaterial.color = Color.black;
        yield return StartCoroutine(HideCover(Color.white));
        pointLight.intensity = 5;
        yield return StartCoroutine(faceWashingScene.Init(activePlayers));
    }

    IEnumerator LoadBubbleTeaDrinking()
    {
        videoPlane.SetActive(false);
        mainCamera.SetActive(true);
        videoCamera.SetActive(false);
        bgMaterial.mainTexture = bgTextures[2];
        // bgCylinder.transform.rotation *= Quaternion.AngleAxis(100, Vector3.up);
        bgCylinder.transform.rotation = Quaternion.Euler(0, 93, 0);
        iTween.RotateTo(bgCylinder, iTween.Hash("rotation", new Vector3(0, 183, 0), "time", 8, "easetype", iTween.EaseType.easeOutSine));
        yield return StartCoroutine(HideCover(Color.white));
        yield return StartCoroutine(bubbleTeaDrinkingScene.Init(activePlayers));
    }

    IEnumerator LoadHazeFanning()
    {
        videoPlane.SetActive(false);
        mainCamera.SetActive(true);
        videoCamera.SetActive(false);
        bgMaterial.mainTexture = bgTextures[3];
        // bgCylinder.transform.rotation *= Quaternion.AngleAxis(60, Vector3.up);
        bgCylinder.transform.rotation = Quaternion.Euler(0, 153, 0);
        iTween.RotateTo(bgCylinder, iTween.Hash("rotation", new Vector3(0, 243, 0), "time", 8, "easetype", iTween.EaseType.easeOutSine));
        yield return StartCoroutine(HideCover(Color.white));
        yield return StartCoroutine(hazeFanningScene.Init(activePlayers));
    }

    IEnumerator LoadWinner()
    {
        videoPlane.SetActive(false);
        mainCamera.SetActive(true);
        videoCamera.SetActive(false);
        bgMaterial.mainTexture = bgTextures[4];
        // bgCylinder.transform.rotation *= Quaternion.AngleAxis(260, Vector3.up);
        bgCylinder.transform.rotation = Quaternion.Euler(0, 53, 0);
        iTween.RotateTo(bgCylinder, iTween.Hash("rotation", new Vector3(0, 143, 0), "time", 8, "easetype", iTween.EaseType.easeOutSine));
        yield return StartCoroutine(HideCover(Color.black));
        int winnerId = 0;
        for (int i = 1; i < scores.Length; i++){
            if (scores[i] > scores[winnerId]) { winnerId = i; }
        }
        winner.Show(winnerId);
        iTween.MoveAdd(gameObject, iTween.Hash("name", "showStarryVideos", "delay", 22.2, "onComplete", "NextScene"));
    }

    public void LoadPuppets(GameObject[] incoming){
        puppets = incoming;
        ranks = new int[puppets.Length];
        currentRank = 1;

        switch(sceneID){
            case 1:
            faceWashingSpawns = new FaceWashingSpawn[puppets.Length];
            for(int i = 0; i < faceWashingSpawns.Length; i++){
                if(puppets[i] != null){
                    faceWashingSpawns[i] = puppets[i].GetComponent<FaceWashingSpawn>();
                }
            }
            break;
            case 3:
            bubbleTeaDrinkingSpawns = new BubbleTeaDrinkingSpawn[puppets.Length];
            for(int i = 0; i < bubbleTeaDrinkingSpawns.Length; i++){
                if(puppets[i] != null){
                    bubbleTeaDrinkingSpawns[i] = puppets[i].GetComponent<BubbleTeaDrinkingSpawn>();
                }
            }
            break;
            case 5:
            hazeFanningSpawns = new HazeFanningSpawn[puppets.Length];
            for(int i = 0; i < hazeFanningSpawns.Length; i++){
                if(puppets[i] != null){
                    hazeFanningSpawns[i] = puppets[i].GetComponent<HazeFanningSpawn>();
                }
            }
            break;
        }
    }

    public void missionComplete(int playerId, Quaternion spawnRotation){
        ranks[playerId] = currentRank;

        GameObject temp = Instantiate(EndTextureObj, EndTextureObj.transform.position, spawnRotation);
        temp.transform.parent = transform;
        switch(sceneID){
            case 1:
            case 3:
                scores[playerId] += currentRank;
                break;
            case 5:
                scores[playerId] -= currentRank;
                break;
        }
        if(currentRank < 4){
            temp.GetComponentInChildren<SpriteRenderer>().sprite = rankSprites[currentRank - 1];
        }
        else{
            temp.GetComponentInChildren<SpriteRenderer>().sprite = rankSprites[3];
        }
        temp.SetActive(true);

        if(currentRank == activePlayerCount){
            iTween.MoveAdd(gameObject, iTween.Hash("name", "missionCompleteNextScene", "delay", 3, "onComplete", "NextScene"));
        }
        currentRank++;
    }

    public void CountdownEnded(){
        gameInProgress = true;
    }

    public void ReceiveArduinoInput(string data){
        if(data.Length != 13) return;
        if(!gameInProgress){
            if(detectPlayers){
                activePlayerCount = 0;
                for(int i = 0; i < 4; i++){
                    string newState = "" + data[i];
                    int stateCheck = int.Parse(newState);
                    if (stateCheck == 1) activePlayerCount++;
                    if(firstSample || int.Parse(activePlayers[i]) != stateCheck){
                        activePlayers[i] = newState;
                        if(stateCheck == 1){
                            rotatingControllers[i].Hide();
                        }
                        else{
                            rotatingControllers[i].Show();
                        }
                    }
                }

                if(firstSample){
                    firstSample = false;
                }

                if(activePlayerCount == 4 || (Input.GetKeyDown(KeyCode.P) && activePlayerCount > 0)){
                    detectPlayers = false;
                    NextScene();
                }
            }
            return;
        }
        switch(sceneID){
            case 1:
            for(int i = 0; i < 4; i++){
                if(activePlayers[i].Equals("1")){
                    faceWashingSpawns[i].OnClick(data[i + 4]);
                }
            }
            break;
            case 3:
            for(int i = 0; i < 4; i++){
                if(activePlayers[i].Equals("1")){
                    bubbleTeaDrinkingSpawns[i].OnClick(data[i + 4]);
                }
            }
            break;
            case 5:
            for(int i = 0; i < 4; i++){
                if(activePlayers[i].Equals("1")){
                    hazeFanningSpawns[i].OnShake(data[i + 8]);
                }
            }
            break;
        }
    }

}
