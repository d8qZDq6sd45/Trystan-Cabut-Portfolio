from flask import Flask, request, jsonify

app = Flask(__name__)

#Vérification de la clé
@app.route("/verifier", methods=["POST"])
def verifier():
    data = request.get_json()
    cle = data.get("cle")
    print(f"[+] Clef reçue : {cle}")
    return jsonify({"valide": cle == "ABC-123"})

if __name__ == "__main__":
    app.run(port=5000)