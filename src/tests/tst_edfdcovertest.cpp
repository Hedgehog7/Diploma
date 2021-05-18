#include <QtTest>
#include "edfd_cover/EDFDCoverInternal.h"
#include "edfd_cover/EDFDCover.h"
#include "ATCore/sdp/SDPDocument.h"
#include "ATCore/utils/helpers.h"
#include <ATProject/AProject.h>

void test_edfd_cover(); //forward declaration, run old tests from previous project, see test_edfd_cover.cpp

class EDFDCoverTest : public QObject
{
    Q_OBJECT

public:
    EDFDCoverTest();
private:
    SDPDocument _sdpDocAT; //SDPs
    EDFDDocument _staticEDFD; //sample EDFD for static IES
    EDFDDocument _dynamicEDFD; //sample EDFD for dynamic IES

    template<typename T>
    bool find_in_vector(const std::vector<T>& v, const T& el) const;

    using connection_t = std::tuple<edfd_element, edfd_element, edfd_connection>;
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testConvertEmptySDP();
    void testConvertSimpleSDP();
    void testConvertRealSDP();

    void testConvertSampleStaticIesEdfd();

    void testPlanningSampleStaticIesEdfd();
    void testPlanningSampleDynamicIesEdfd();
    void testPlanningTwoInstances();

    void testLegacyTests(); //run tests from previous project
};

EDFDCoverTest::EDFDCoverTest()
{
}

template<typename T>
bool EDFDCoverTest::find_in_vector(const std::vector<T>& v, const T& el) const
{
    auto f_it = std::find(v.begin(), v.end(), el);
    return f_it != v.end();
}

void EDFDCoverTest::initTestCase()
{
    //preload real SDPs and some project templates
    std::string sdp_path = "kb/procedures.xml";
    if (_sdpDocAT.loadFromFile(sdp_path).OK())
        qDebug() << sdp_path.c_str() << " loaded successfuly\n";
    else
        qDebug() << sdp_path.c_str() << " load failed\n";

    std::string edfd_path = "templates/SimpleIES/main.edfd";
    if (_staticEDFD.loadFromFile(edfd_path).OK())
        qDebug() << edfd_path.c_str() << " loaded successfuly\n";
    else
        qDebug() << edfd_path.c_str() << " load failed\n";

    std::string dyn_edfd_path = "templates/DynamicIES/main.edfd";
    if (_dynamicEDFD.loadFromFile(dyn_edfd_path).OK())
        qDebug() << dyn_edfd_path.c_str() << " loaded successfuly\n";
    else
        qDebug() << dyn_edfd_path.c_str() << " load failed\n";
}

void EDFDCoverTest::cleanupTestCase()
{
}

void EDFDCoverTest::testConvertEmptySDP()
{
    //create SDP without content
    SDP emptySDP;
    //convert this SDP to planner's internal SDP representation
    PlannerSDPRepresentation result(emptySDP);

    //check if there is no main part, because SDP was empty
    QVERIFY(result.getMainPartEDFD().empty());
    //check if there are no optional parts, because SDP was empty
    QVERIFY(result.getOptionalParts().empty());
}

void EDFDCoverTest::testConvertSimpleSDP()
{
    //create SDP with one fragment (main)
    SDP simpleSDP;
    simpleSDP.add_node(0, "One", SDP::Node::Type::Entity);
    simpleSDP.add_node(1, "Two", SDP::Node::Type::Entity);
    simpleSDP.add_flow("Connection", 0, 1);

    SDP::Fragment frag;
    frag.id = 0;
    frag.name = "First";
    frag.primary = true;
    frag.node_ids.insert(0);
    frag.node_ids.insert(1);
    simpleSDP.add_fragment(frag);

    //convert this SDP to planner's internal SDP representation
    PlannerSDPRepresentation result(simpleSDP);

    //check if there is main part, because SDP has one
    QVERIFY(!result.getMainPartEDFD().empty());
    //check if there are no optional parts, because SDP has only main part
    QVERIFY(result.getOptionalParts().empty());

    //create edfd graph expected to be main part of SDP above
    edfd_element one {0, "One", edfd_element::type_t::entity};
    edfd_element two {1, "Two", edfd_element::type_t::entity};
    edfd_graph expectedGraph {
        { one, two },
        {
            connection_t {one, two, "Connection"}
        }
    };

    //check if converted SDP has main part same to created manually
    QVERIFY(expectedGraph == result.getMainPartEDFD());
}

void EDFDCoverTest::testConvertRealSDP()
{
    //check if loaded file has 2 SDPs (one static and one dynamic at this monent)
    //when SDPs are add to this file, this test should be updated
    QCOMPARE(2UL, _sdpDocAT.getSDPs().size());

    //take static SDP, assume it is the first SDP in file
    const SDP& staticSDP = _sdpDocAT.getSDPs().front();

    //convert this SDP to planner's internal SDP representation
    PlannerSDPRepresentation result(staticSDP);

    //check if there is main part, because SDP has one
    QVERIFY(!result.getMainPartEDFD().empty());
    //check if there are 3 optional parts, because SDP has them (db integration, apps integration, explanations)
    QCOMPARE(3UL, result.getOptionalParts().size());

    //collect optional parts' EDFD graphs to vector
    std::vector<edfd_graph> optionaFragmentsEDFD;
    for (auto it : result.getOptionalParts())
        optionaFragmentsEDFD.push_back(it.first);

    //expected results
    edfd_element db{ 7, "База данных", edfd_element::type_t::storage };
    edfd_element receive_data{ 8, "Получить данные", edfd_element::type_t::process };

    edfd_graph db_integration_sdp(
        { db, receive_data },
        {
            connection_t{ db, receive_data, "данные" },
        }
    );

    //check if there is db integration fragment
    QVERIFY(find_in_vector(optionaFragmentsEDFD, db_integration_sdp));

    edfd_element app{ 6, "Пакет прикладных программ", edfd_element::type_t::entity };
    edfd_element calc{ 5, "Выполнить расчет", edfd_element::type_t::process };

    edfd_graph app_integration_sdp(
        { app, calc },
        {
            connection_t{ app, calc, "результат" },
            connection_t{ calc, app, "данные" }
        }
    );

    //check if there is apps integration fragment
    QVERIFY(find_in_vector(optionaFragmentsEDFD, app_integration_sdp));

    edfd_element explain{ 3, "Объяснить", edfd_element::type_t::process };
    edfd_graph explanations(
        { explain },
        std::initializer_list<connection_t> {}
    );

    //check if there is explanations integration fragment
    QVERIFY(find_in_vector(optionaFragmentsEDFD, explanations));

    edfd_element nf_operation{ 0, "Статическая НФ-операция", edfd_element::type_t::nf_process };
    edfd_element setup_situation{ 1, "Задать начальную ситуацию", edfd_element::type_t::process };
    edfd_element show_recommendations{ 2, "Отобразить рекомендации", edfd_element::type_t::process };

    edfd_graph main_part_sdp(
        {nf_operation, setup_situation, show_recommendations},
        {
            connection_t{ setup_situation, nf_operation, "факты" },
            connection_t{ nf_operation, show_recommendations, "результат вывода" },
        }
    );

    //check main part
    QCOMPARE(main_part_sdp, result.getMainPartEDFD());
}

void EDFDCoverTest::testConvertSampleStaticIesEdfd()
{
    edfd_graph result = convertDocumentToGraph(&_staticEDFD);

    QCOMPARE(9UL, result.get_vertices().size());

    //expected result
    edfd_element nf_operation        { 0, "Статическая НФ-операция",    edfd_element::type_t::nf_process};
    edfd_element db                  { 1, "База данных",                edfd_element::type_t::storage   };
    edfd_element receive_data        { 2, "Получить данные",            edfd_element::type_t::process   };
    edfd_element calc                { 3, "Выполнить расчет",           edfd_element::type_t::process   };
    edfd_element app                 { 4, "Пакет прикладных программ",  edfd_element::type_t::entity    };
    edfd_element show_recommendations{ 5, "Отобразить рекомендации",    edfd_element::type_t::process   };
    edfd_element setup_situation     { 6, "Задать начальную ситуацию",  edfd_element::type_t::process   };
    edfd_element user                { 7, "Пользователь",               edfd_element::type_t::entity    };
    edfd_element explain             { 8, "Объяснить",                  edfd_element::type_t::process   };
    edfd_graph expected_graph(
        { nf_operation, setup_situation, show_recommendations, explain, user, calc, app, receive_data, db },
        {
            connection_t{ db, receive_data, "данные" },
            connection_t{ receive_data, nf_operation, "данные" },
            connection_t{ nf_operation, receive_data, "запросы" },
            connection_t{ calc, app, "данные" },
            connection_t{ app, calc, "результат" },
            connection_t{ nf_operation, calc, "данные" },
            connection_t{ calc, nf_operation, "результат расчета" },
            connection_t{ nf_operation, explain, "правила" },
            connection_t{ nf_operation, explain, "трасса вывода" },
            connection_t{ explain, user, "объяснения" },
            connection_t{ setup_situation, nf_operation, "факты" },
            connection_t{ nf_operation, show_recommendations, "результат вывода" },
            connection_t{ user, setup_situation, "данные" },
            connection_t{ show_recommendations, user, "рекомендации" },
        }
    );

    QCOMPARE(result, expected_graph);
}

std::ostream& operator << (std::ostream& os, APlanTask* task)
{
    return os << task->name << "(" << task->stage << ")";
}

void EDFDCoverTest::testPlanningSampleStaticIesEdfd()
{
    AProject proj;
    APlan* plan = EDFDCover::buildPlan(&proj, &_staticEDFD, _sdpDocAT.getSDPs());

    //std::cout << "Plan tasks dependency graph:\n";
    //print_explicit_graph(plan->predcess, std::cout, 4);

    AHierarchyPlanPresentation::hierarchy_comparator comparator(reversed_topological_sort(plan->predcess));

    std::cout << "Top sort:";
    for (auto v : comparator._top_sort)
        std::cout << v << "; ";
    std::cout << std::endl;

    std::cout << "Top sort stages:";
    for (auto v : comparator._stages_sorted)
        std::cout << v << "; ";
    std::cout << std::endl;
}

void EDFDCoverTest::testPlanningSampleDynamicIesEdfd()
{
    AProject proj;
    APlan* plan = EDFDCover::buildPlan(&proj, &_dynamicEDFD, _sdpDocAT.getSDPs());

    //std::cout << "Plan tasks dependency graph:\n";
    //print_explicit_graph(plan->predcess, std::cout, 4);

    AHierarchyPlanPresentation::hierarchy_comparator comparator(reversed_topological_sort(plan->predcess));

    std::cout << "Top sort:";
    for (auto v : comparator._top_sort)
        std::cout << v << "; ";
    std::cout << std::endl;

    std::cout << "Top sort stages:";
    for (auto v : comparator._stages_sorted)
        std::cout << v << "; ";
    std::cout << std::endl;
}

void EDFDCoverTest::testPlanningTwoInstances()
{
    EDFDDocument twoInstancesDoc;
    QVERIFY(twoInstancesDoc.loadFromFile("test_data/TwoInstances.edfd").OK());

    AProject proj;
    APlan* plan = EDFDCover::buildPlan(&proj, &twoInstancesDoc, _sdpDocAT.getSDPs());

    //std::cout << "Plan tasks dependency graph:\n";
    //print_explicit_graph(plan->predcess, std::cout, 4);

    AHierarchyPlanPresentation::hierarchy_comparator comparator(reversed_topological_sort(plan->predcess));

    std::cout << "Top sort:";
    for (auto v : comparator._top_sort)
        std::cout << v << "; ";
    std::cout << std::endl;

    std::cout << "Top sort stages:";
    for (auto v : comparator._stages_sorted)
        std::cout << v << "; ";
    std::cout << std::endl;
}

void EDFDCoverTest::testLegacyTests()
{
    test_edfd_cover();
}

QTEST_APPLESS_MAIN(EDFDCoverTest)

#include "tst_edfdcovertest.moc"
