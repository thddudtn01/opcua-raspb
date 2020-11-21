#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/pki_default.h>

UA_Boolean running = true;
int shmid;
char *shmaddr;
struct shmid_ds shm_stat;

void shmemHandler(){
    if((shmid=shmget((key_t)777, 10, IPC_CREAT|0666)) == -1) {
       perror("shmid failed");
       exit(1);
    }
    if((shmaddr=shmat(shmid, (void *)0, 0)) == (void *)-1) {
       perror("shmat failed");
       exit(1);
    }
    printf("data read from shared memory : %s\n", (char *)shmaddr);
    if(shmctl(shmid, IPC_STAT, &shm_stat) == -1) {
       perror("shmctl failed");
       exit(1);
    }
}

void signalHandler(int signo){
    if(shmctl(shmid, IPC_RMID, 0) == -1) {
       perror("shmctl failed");
       exit(1);
    }
    exit(0);
}

static UA_StatusCode
readTraffic(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionContext,
    const UA_NodeId* nodeId, void* nodeContext,
    UA_Boolean sourceTimeStamp,
    const UA_NumericRange* range, UA_DataValue* value) {
    UA_Int32 toggle = (UA_Int32)(shmaddr[nodeId->identifier.numeric - 43000]-'0');
    UA_Variant_setScalarCopy(&value->value, &toggle, &UA_TYPES[UA_TYPES_INT32]);
    value->hasValue = true;
    if (sourceTimeStamp) {
        value->hasSourceTimestamp = true;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
writeTraffic(UA_Server *Server,
                 const UA_NodeId *sessionId, void *sessionContext,
                 const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {;
    shmaddr[nodeId->identifier.numeric - 43000] = *(char*)(data->value.data)+'0';
    return UA_STATUSCODE_GOOD;
}

static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

int main(int argc, char** argv) {
    UA_StatusCode retval;
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server* server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    shmemHandler();

    UA_DataSource trafficDataSource;
    trafficDataSource.read = NULL;
    trafficDataSource.write = NULL;
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    trafficDataSource.read = readTraffic;
    trafficDataSource.write = writeTraffic;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "green");
    UA_QualifiedName qualifiedName = UA_QUALIFIEDNAME(1, "green");
    retval = UA_Server_addDataSourceVariableNode(server, UA_NODEID_NUMERIC(1, 43000),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qualifiedName,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr, trafficDataSource, NULL, NULL);

    trafficDataSource.read = NULL;
    trafficDataSource.write = NULL;
    attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    trafficDataSource.read = readTraffic;
    trafficDataSource.write = writeTraffic;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "yellow");
    qualifiedName = UA_QUALIFIEDNAME(1, "yellow");
    retval = UA_Server_addDataSourceVariableNode(server, UA_NODEID_NUMERIC(1, 43001),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qualifiedName,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr, trafficDataSource, NULL, NULL);

    trafficDataSource.read = NULL;
    trafficDataSource.write = NULL;
    attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    trafficDataSource.read = readTraffic;
    trafficDataSource.write = writeTraffic;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "red");
    qualifiedName = UA_QUALIFIEDNAME(1, "red");
    retval = UA_Server_addDataSourceVariableNode(server, UA_NODEID_NUMERIC(1, 43002),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qualifiedName,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        attr, trafficDataSource, NULL, NULL);

    retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return (int)retval;
}
