using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Winner : MonoBehaviour{
    public Material[] playerColourMaterials;
    
    public MeshRenderer coverMesh;
    public MeshRenderer bodyMesh;

    public GameObject winnerModel;
    public GameObject noMicroBead;
    public GameObject noStraw;
    public GameObject noForestFire;

    public void Show(int playerID){
        coverMesh.material = playerColourMaterials[playerID];
        bodyMesh.material = playerColourMaterials[playerID];
        iTween.ScaleTo(winnerModel, iTween.Hash("scale", Vector3.one, "time", 0.5f));
        iTween.ScaleTo(winnerModel, iTween.Hash("scale", Vector3.zero, "time", 0.5f, "delay", 8));
        iTween.ScaleTo(noMicroBead, iTween.Hash("scale", Vector3.one, "time", 0.5f, "delay", 8));
        iTween.ScaleTo(noStraw, iTween.Hash("scale", Vector3.one, "time", 0.5f, "delay", 10));
        iTween.ScaleTo(noForestFire, iTween.Hash("scale", Vector3.one, "time", 0.5f, "delay", 12));
        iTween.ScaleTo(noMicroBead, iTween.Hash("scale", Vector3.zero, "time", 0.5f, "delay", 22));
        iTween.ScaleTo(noStraw, iTween.Hash("scale", Vector3.zero, "time", 0.5f, "delay", 22));
        iTween.ScaleTo(noForestFire, iTween.Hash("scale", Vector3.zero, "time", 0.5f, "delay", 22, 
            "oncomplete", "DestroySelf", "oncompletetarget", gameObject));
    }

    void DestroySelf(){
        Destroy(gameObject);
    }
}
