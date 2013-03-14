import qbs.base 1.0

Project {   
    moduleSearchPaths: [ "qbs/modules", "modules" ]

    references: [
        "src/3rdparty/k8json/k8json.qbs",
        "src/api.qbs",
        "src/oauth/oauth.qbs",
        "src/qml/qml.qbs",
        "src/directauth/directauth.qbs",
//        "examples/examples.qbs"
    ]
}
