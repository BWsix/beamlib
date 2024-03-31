#include <beamlib.h>

#include "model.h"

#include "shaderProgramStore.h"
#include "cube.hpp"
#include "star.hpp"
#include "grid.hpp"
#include "bot.hpp"

const char *idle_str = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"thumb2","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999991059303284],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999997615814209],"scale":[1.5,1.0,1.5]}}],"data":null,"name":"thumb1","transform":{"localRotationQuat":[-0.4997842013835907,-0.5145970582962036,-0.4997841715812683,-0.48539769649505615],"position":[1.0499999523162842,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.7000000476837158,1.0,1.7000000476837158]}},{"children":[{"children":[{"children":[],"data":null,"name":"index3","transform":{"localRotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999985694885254],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"index2","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999995231628418],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"index1","transform":{"localRotationQuat":[0.5008605718612671,0.5135514140129089,0.5008606910705566,0.4842890202999115],"position":[0.5,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"middle3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"middle2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,1.0,1.399999976158142]}}],"data":null,"name":"middle1","transform":{"localRotationQuat":[0.49978679418563843,0.5145997405052185,0.49978679418563843,0.4854002296924591],"position":[-0.05000000074505806,0.0,-1.100000023841858],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,1.0,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"ring3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"ring2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"ring1","transform":{"localRotationQuat":[0.49978679418563843,0.5145997405052185,0.49978679418563843,0.4854002296924591],"position":[-0.550000011920929,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"pinky3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.4000000059604645,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.100000023841858,0.44999998807907104,1.100000023841858]}}],"data":null,"name":"pinky2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2000000476837158,0.550000011920929,1.2000000476837158]}}],"data":null,"name":"pinky1","transform":{"localRotationQuat":[0.49978679418563843,0.5145997405052185,0.49978679418563843,0.4854002296924591],"position":[-1.0,0.0,-0.44999998807907104],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.75,1.2999999523162842]}}],"data":null,"name":"palm","transform":{"localRotationQuat":[0.0499790795147419,0.0,0.0,0.9987484216690063],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[7.75,0.44999998807907104,9.0]}}],"data":null,"name":"Hand Manager","transform":{"localRotationQuat":[0.0,0.0,0.0,0.999998927116394],"position":[0.0,0.3499999940395355,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *rock_str = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"thumb2","transform":{"localRotationQuat":[0.45639291405677795,0.2957359850406647,0.4910367429256439,0.6805305480957031],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999997615814209],"scale":[1.5,1.0,1.5]}}],"data":null,"name":"thumb1","transform":{"localRotationQuat":[-0.6363927721977234,-0.0703011155128479,-0.7629148364067078,0.08950050920248032],"position":[1.0499999523162842,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.7000000476837158,1.0,1.7000000476837158]}},{"children":[{"children":[{"children":[],"data":null,"name":"index3","transform":{"localRotationQuat":[-1.587949993719917e-09,9.73580549512576e-10,0.5226863622665405,0.8525230884552002],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"index2","transform":{"localRotationQuat":[0.0,0.0,0.6442168354988098,0.7648412585258484],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"index1","transform":{"localRotationQuat":[0.586866021156311,0.1477763056755066,0.7771231532096863,0.17269471287727356],"position":[0.5,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"middle3","transform":{"localRotationQuat":[0.0,0.0,0.6248969435691833,0.7807064652442932],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"middle2","transform":{"localRotationQuat":[0.0,0.0,0.6442173719406128,0.7648418545722961],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,1.0,1.399999976158142]}}],"data":null,"name":"middle1","transform":{"localRotationQuat":[0.713774561882019,0.0715891569852829,0.6949630379676819,0.04925656318664551],"position":[-0.05000000074505806,0.0,-1.100000023841858],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,1.0,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"ring3","transform":{"localRotationQuat":[0.0,0.0,0.7173554301261902,0.6967060565948486],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"ring2","transform":{"localRotationQuat":[0.0,0.0,0.5226870179176331,0.8525241017341614],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"ring1","transform":{"localRotationQuat":[0.8231621980667114,0.06539924442768097,0.5638760924339294,-0.013003039173781872],"position":[-0.550000011920929,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"pinky3","transform":{"localRotationQuat":[0.0,0.0,0.4349651336669922,0.900446355342865],"position":[0.0,-0.4000000059604645,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.100000023841858,0.44999998807907104,1.100000023841858]}}],"data":null,"name":"pinky2","transform":{"localRotationQuat":[0.0,0.0,0.7345468997955322,0.6785561442375183],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2000000476837158,0.550000011920929,1.2000000476837158]}}],"data":null,"name":"pinky1","transform":{"localRotationQuat":[0.8985009789466858,-0.056951917707920074,0.4258475601673126,-0.09002269059419632],"position":[-1.0,0.0,-0.44999998807907104],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.75,1.2999999523162842]}}],"data":null,"name":"palm","transform":{"localRotationQuat":[0.0499790795147419,0.0,0.0,0.9987484216690063],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[7.75,0.44999998807907104,9.0]}}],"data":null,"name":"Hand Manager","transform":{"localRotationQuat":[0.45733770728111267,0.0,0.0,0.8892912864685059],"position":[0.0,1.0499999523162842,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *paper_str = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"thumb2","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999991059303284],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999997615814209],"scale":[1.5,1.0,1.5]}}],"data":null,"name":"thumb1","transform":{"localRotationQuat":[-0.39728426933288574,-0.30608636140823364,-0.6848112344741821,-0.5286762714385986],"position":[1.0499999523162842,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.7000000476837158,1.0,1.7000000476837158]}},{"children":[{"children":[{"children":[],"data":null,"name":"index3","transform":{"localRotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999985694885254],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"index2","transform":{"localRotationQuat":[0.0,0.0,0.0,0.9999995231628418],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"index1","transform":{"localRotationQuat":[0.46406614780426025,0.38844797015190125,0.6245176792144775,0.49368149042129517],"position":[0.5,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"middle3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"middle2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,1.0,1.399999976158142]}}],"data":null,"name":"middle1","transform":{"localRotationQuat":[0.5485941171646118,0.4622160792350769,0.5456836819648743,0.43316179513931274],"position":[-0.05000000074505806,0.0,-1.100000023841858],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,1.0,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"ring3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"ring2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"ring1","transform":{"localRotationQuat":[0.6156812310218811,0.5502814054489136,0.45673415064811707,0.33093690872192383],"position":[-0.550000011920929,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"pinky3","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.4000000059604645,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.100000023841858,0.44999998807907104,1.100000023841858]}}],"data":null,"name":"pinky2","transform":{"localRotationQuat":[0.0,0.0,0.0,1.0],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2000000476837158,0.550000011920929,1.2000000476837158]}}],"data":null,"name":"pinky1","transform":{"localRotationQuat":[0.6514294743537903,0.6214393973350525,0.35505199432373047,0.25177040696144104],"position":[-1.0,0.0,-0.44999998807907104],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.75,1.2999999523162842]}}],"data":null,"name":"palm","transform":{"localRotationQuat":[0.0499790795147419,0.0,0.0,0.9987484216690063],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[7.75,0.44999998807907104,9.0]}}],"data":null,"name":"Hand Manager","transform":{"localRotationQuat":[0.47942501306533813,0.0,0.0,0.8775815963745117],"position":[0.0,0.3499999940395355,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

const char *scissor_str = R"json(
{"children":[{"children":[{"children":[{"children":[],"data":null,"name":"thumb2","transform":{"localRotationQuat":[0.45639291405677795,0.2957359850406647,0.4910367429256439,0.6805305480957031],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[-1.862645149230957e-09,0.0,0.0,0.9999997615814209],"scale":[1.5,1.0,1.5]}}],"data":null,"name":"thumb1","transform":{"localRotationQuat":[-0.6363927721977234,-0.0703011155128479,-0.7629148364067078,0.08950050920248032],"position":[1.0499999523162842,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.7000000476837158,1.0,1.7000000476837158]}},{"children":[{"children":[{"children":[],"data":null,"name":"index3","transform":{"localRotationQuat":[0.04997904598712921,5.544174416041973e-17,-2.7743993432691946e-18,0.9987478256225586],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"index2","transform":{"localRotationQuat":[0.17410780489444733,0.0,0.0,0.9847246408462524],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"index1","transform":{"localRotationQuat":[-0.7093076705932617,0.10405558347702026,-0.10713961720466614,-0.6888899803161621],"position":[0.5,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"middle3","transform":{"localRotationQuat":[0.09983333945274353,0.0,0.0,0.9950034618377686],"position":[0.0,-0.800000011920929,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"middle2","transform":{"localRotationQuat":[0.1494380384683609,0.0,0.0,0.9887704253196716],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,1.0,1.399999976158142]}}],"data":null,"name":"middle1","transform":{"localRotationQuat":[0.7172002792358398,0.014914349652826786,0.014485035091638565,0.6965553760528564],"position":[-0.05000000074505806,0.0,-1.100000023841858],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,1.0,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"ring3","transform":{"localRotationQuat":[0.0,0.0,0.7173554301261902,0.6967060565948486],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.6000000238418579,1.2999999523162842]}}],"data":null,"name":"ring2","transform":{"localRotationQuat":[0.0,0.0,0.5226870179176331,0.8525241017341614],"position":[0.0,-0.699999988079071,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.4500000476837158,0.75,1.399999976158142]}}],"data":null,"name":"ring1","transform":{"localRotationQuat":[0.8231621980667114,0.06539924442768097,0.5638760924339294,-0.013003039173781872],"position":[-0.550000011920929,0.0,-0.8999999761581421],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.5,0.8999999761581421,1.5]}},{"children":[{"children":[{"children":[],"data":null,"name":"pinky3","transform":{"localRotationQuat":[0.0,0.0,0.6631343364715576,0.7484981417655945],"position":[0.0,-0.4000000059604645,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.100000023841858,0.44999998807907104,1.100000023841858]}}],"data":null,"name":"pinky2","transform":{"localRotationQuat":[0.0,0.0,0.5226861834526062,0.8525227904319763],"position":[0.0,-0.6499999761581421,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2000000476837158,0.550000011920929,1.2000000476837158]}}],"data":null,"name":"pinky1","transform":{"localRotationQuat":[0.8985009789466858,-0.056951917707920074,0.4258475601673126,-0.09002269059419632],"position":[-1.0,0.0,-0.44999998807907104],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.2999999523162842,0.75,1.2999999523162842]}}],"data":null,"name":"palm","transform":{"localRotationQuat":[0.0499790720641613,6.293179467320442e-06,-0.00012576207518577576,0.9987483024597168],"position":[0.0,0.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[7.75,0.44999998807907104,9.0]}}],"data":null,"name":"Hand Manager","transform":{"localRotationQuat":[0.501211941242218,-6.246846169233322e-05,-1.1043434824387077e-05,0.8653221130371094],"position":[0.0,1.0,0.0],"rotationQuat":[0.0,0.0,0.0,1.0],"scale":[1.0,1.0,1.0]}}
)json";

class Animator {
    float startTime = 0.0f;
    float progress = 0.0f;

    bool playing = false;
    float pauseStartTime = 0.0f;

    float frameStartTime = 0.0f;
    int frameIdx = 0;

public:
    float looping = false;
    std::vector<json> frames;

    bool isPlaying() { return playing; }

    void Pause() {
        playing = false;
        pauseStartTime = beamlib::getTimeElapsed();
    }
    void Resume() {
        playing = true;
        frameStartTime += beamlib::getTimeElapsed() - pauseStartTime;
    }

    void Update(beamlib::Instance *root) {
        if (!playing) return;
        if (!looping && (frameIdx % frames.size()) == frames.size() - 1) {
            End();
            return;
        }

        float deltaTime = beamlib::getTimeElapsed() - frameStartTime;
        if (deltaTime < 0.5f) {
            progress = deltaTime / 0.5f;
        } else {
            frameStartTime = beamlib::getTimeElapsed();
            frameIdx += 1;
            return;
        }

        json frame = root->Interpolator(getFrame(frameIdx), getFrame(frameIdx + 1), progress);
        root->Load(frame);
    }

    json getFrame(int idx) {
        idx %= frames.size();
        return frames[idx];
    }

    void Play() {
        startTime = beamlib::getTimeElapsed();
        frameStartTime = beamlib::getTimeElapsed();
        playing = true;
        progress = 0.0f;
        frameIdx = 0;
    }

    void End() {
        playing = false;
        progress = 1.0f;
    }
};

int main(int argc, char **argv) {
    const auto window = beamlib::CreateWindow("beamlib");
    store::Init();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    beamlib::camera.transform.Translate({0, 0, 10});

    Cube cube;
    Star star;

    beamlib::DummyInstance root(NULL, "Root");
    root.PushChild(&beamlib::camera);

    Grid gridObject;
    GridInstance gridXZ(&gridObject, "Grid XZ");
    root.PushChild(&gridXZ);

    Animator rock, paper, scissor, idle;
    idle.frames.push_back(json::parse(idle_str));
    idle.frames.push_back(json::parse(idle_str));
    rock.frames.push_back(json::parse(idle_str));
    rock.frames.push_back(json::parse(rock_str));
    paper.frames.push_back(json::parse(idle_str));
    paper.frames.push_back(json::parse(paper_str));
    scissor.frames.push_back(json::parse(idle_str));
    scissor.frames.push_back(json::parse(scissor_str));
    Animator animator;

    beamlib::DummyInstance handManager(NULL, "Hand Manager");
    root.PushChild(&handManager);

    // BotBodyUpper botBodyUpperObject;
    // BotBodyLower botBodyLowerObject;
    // BotLid botLidObject;
    BotArmUpper botArmUpperObject;
    // BotArmLower botArmLowerObject;
    // BotLeg botLegObject;

    BotArmUpperInstance palm(&botArmUpperObject, "palm");
        handManager.PushChild(&palm);
        BotArmUpperInstance thumb1(&botArmUpperObject, "thumb1");
            palm.PushChild(&thumb1);
            BotArmUpperInstance thumb2(&botArmUpperObject, "thumb2");
                thumb1.PushChild(&thumb2);
        BotArmUpperInstance index1(&botArmUpperObject, "index1");
            palm.PushChild(&index1);
            BotArmUpperInstance index2(&botArmUpperObject, "index2");
                index1.PushChild(&index2);
                BotArmUpperInstance index3(&botArmUpperObject, "index3");
                    index2.PushChild(&index3);
        BotArmUpperInstance middle1(&botArmUpperObject, "middle1");
            palm.PushChild(&middle1);
            BotArmUpperInstance middle2(&botArmUpperObject, "middle2");
                middle1.PushChild(&middle2);
                BotArmUpperInstance middle3(&botArmUpperObject, "middle3");
                    middle2.PushChild(&middle3);
        BotArmUpperInstance ring1(&botArmUpperObject, "ring1");
            palm.PushChild(&ring1);
            BotArmUpperInstance ring2(&botArmUpperObject, "ring2");
                ring1.PushChild(&ring2);
                BotArmUpperInstance ring3(&botArmUpperObject, "ring3");
                    ring2.PushChild(&ring3);
        BotArmUpperInstance pinky1(&botArmUpperObject, "pinky1");
            palm.PushChild(&pinky1);
            BotArmUpperInstance pinky2(&botArmUpperObject, "pinky2");
                pinky1.PushChild(&pinky2);
                BotArmUpperInstance pinky3(&botArmUpperObject, "pinky3");
                    pinky2.PushChild(&pinky3);

    animator = idle;
    animator.Play();

    // botBodyUpper.Load(json::parse(frame_left));
    while (!beamlib::WindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            animator.End();
            animator = rock;
            animator.Play();
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            animator.End();
            animator = paper;
            animator.Play();
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            animator.End();
            animator = scissor;
            animator.Play();
        }
        animator.Update(&handManager);

        root.Update();
        root.Render();

        beamlib::BeginUI();

        ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("fps: %.0f", beamlib::getFrameRate());
        ImGui::Text("frametime: %f", beamlib::getDeltaTime());
        ImGui::Text("Looping: %s", animator.looping ? "Yes" : "No");
        ImGui::Text("Playing: %s", animator.isPlaying() ? "Yes" : "No");
        ImGui::End();

        ImGui::Begin("Object Hierarchy");
        root.RenderUI();
        ImGui::End();
        beamlib::EndUI();
    }

    beamlib::DestroyWindow(window);

    return 0;
}
