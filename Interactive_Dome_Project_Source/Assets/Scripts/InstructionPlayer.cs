using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InstructionPlayer : MonoBehaviour
{
    public GameObject rotatingChild;
    public GameObject pokingFinger;
    public GameObject shakingArrows;

    public Material[] playerColourMaterials;

    public MeshRenderer coverMesh;
    public MeshRenderer bodyMesh;

    private int playerID;

    void Start(){
        iTween.Init(gameObject);
        iTween.Init(rotatingChild);
    }

    public void SetPlayerId(int id){
        playerID = id;
    }

    public IEnumerator Show(bool isClicking){
        coverMesh.material = playerColourMaterials[playerID];
        bodyMesh.material = playerColourMaterials[playerID];
        GameObject controllerModel = transform.GetChild(0).gameObject;
        iTween.ScaleTo(controllerModel, iTween.Hash("scale", Vector3.one, "time", 0.5f));
        iTween.ScaleTo(controllerModel, iTween.Hash("scale", Vector3.zero, "time", 0.5f, "delay", 5, 
            "oncomplete", "DestroySelf", "oncompletetarget", gameObject));
        yield return new WaitForSeconds(0.5f);

        if(isClicking){
            pokingFinger.SetActive(true);
            iTween.MoveAdd(pokingFinger, iTween.Hash("amount", new Vector3(0.5f, 0.3f, 0),
                "time", 0.2f, "looptype", iTween.LoopType.pingPong, "easetype", iTween.EaseType.linear));
            pokingFinger.transform.parent = controllerModel.transform;
        }
        else{
            shakingArrows.SetActive(true);
            iTween.RotateBy(rotatingChild, iTween.Hash("z", 0.05f, "time", 0.2f,
                "looptype", iTween.LoopType.pingPong, "easetype", iTween.EaseType.linear));
            shakingArrows.transform.parent = controllerModel.transform;
        }
    }

    void DestroySelf(){
        Destroy(gameObject);
    }
}
