#include <gtest/gtest.h>
#include <ArduinoJson/JsonArray.h>
#include <ArduinoJson/JsonObject.h>
#include <ArduinoJson/JsonValue.h>
#include <ArduinoJson/StaticJsonBuffer.h>

using namespace ArduinoJson::Generator;

struct Person 
{
    int id;
    char name[32];
};

class Issue10 : public testing::Test 
{
protected:

    virtual void SetUp()
    {
        Person boss;
        boss.id = 1;
        strcpy(boss.name, "Jeff");
        Person employee;
        employee.id = 2;
        strcpy(employee.name, "John");
        persons[0] = boss;
        persons[1] = employee;
    }

    void checkJsonString(JsonContainer& p)
    {
        char buffer[256];
        p.printTo(buffer, sizeof(buffer));

        EXPECT_STREQ("[{\"id\":1,\"name\":\"Jeff\"},{\"id\":2,\"name\":\"John\"}]", buffer);
    }

    void nodeCountMustBe(int expected)
    {
        EXPECT_EQ(expected, json.size());
    }

    Person persons[2];  
    StaticJsonBuffer<20> json;    
};

TEST_F(Issue10, PopulateArrayByAddingAnObject)
{
    JsonArray array= json.createArray();

    for (int i = 0; i < 2; i++)
    {
        JsonObject object = json.createObject();
        
        object["id"] = persons[i].id;
        object["name"] = persons[i].name;

        array.add(object); // <- adds a reference to an existing objet (creates 2 extra proxy nodes)
    }

    checkJsonString(array);
    nodeCountMustBe(15);
}

TEST_F(Issue10, PopulateArrayByCreatingNestedObjects)
{
    JsonArray array = json.createArray();

    for (int i = 0; i < 2; i++) 
    {
        JsonObject object = array.createNestedObject();

        object["id"] = persons[i].id;
        object["name"] = persons[i].name;
    }

    checkJsonString(array);
    nodeCountMustBe(11);
}