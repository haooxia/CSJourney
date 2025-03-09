// 使用Spring Boot定义一个/recommend的REST API。
@RestController
public class RecommendationController {

    @Autowired
    private RecommendationService recommendationService;

    @PostMapping("/recommend")
    public List<Integer> recommend(@RequestBody UserContent userContent) {
        return recommendationService.getRecommendations(userContent);
    }
}


// 这个类会负责调用Jieba分词、加载Word2Vec模型、从Redis中加载店铺特征，并计算余弦相似度。
@Service
public class RecommendationService {

    @Autowired
    private RedisTemplate<String, byte[]> redisTemplate;

    private Word2Vec word2VecModel;

    // 加载Word2Vec模型（在初始化时调用）
    @PostConstruct
    public void loadWord2VecModel() throws IOException {
        // 假设模型保存在本地路径
        File modelFile = new File("path/to/word2vec-model.bin");
        this.word2VecModel = WordVectorSerializer.readWord2VecModel(modelFile);
    }

    public List<Integer> getRecommendations(UserContent userContent) {
        // Step 1: Jieba分词
        List<String> tokens = jiebaTokenize(userContent.getText());

        // Step 2: 提取用户特征向量
        INDArray userVector = getFeatureVector(tokens);

        // Step 3: 从Redis中获取所有店铺的特征向量
        Map<Integer, INDArray> storeVectors = loadStoreVectorsFromRedis();

        // Step 4: 计算余弦相似度并推荐店铺
        return recommendStores(userVector, storeVectors);
    }

    private List<String> jiebaTokenize(String content) {
        JiebaSegmenter segmenter = new JiebaSegmenter();
        return segmenter.sentenceProcess(content);
    }

    private INDArray getFeatureVector(List<String> words) {
        List<Double> vector = new ArrayList<>();
        for (String word : words) {
            if (word2VecModel.hasWord(word)) {
                INDArray wordVector = word2VecModel.getWordVectorMatrix(word);
                if (vector.isEmpty()) {
                    vector.addAll(Arrays.stream(wordVector.toDoubleVector()).boxed().collect(Collectors.toList()));
                } else {
                    for (int i = 0; i < wordVector.length(); i++) {
                        vector.set(i, vector.get(i) + wordVector.getDouble(i));
                    }
                }
            }
        }
        if (vector.isEmpty()) {
            return Nd4j.zeros(word2VecModel.getWordVectorMatrix("default").length());
        }
        return Nd4j.create(vector.stream().mapToDouble(Double::doubleValue).toArray());
    }

    private Map<Integer, INDArray> loadStoreVectorsFromRedis() {
        Map<Integer, INDArray> storeVectors = new HashMap<>();
        // 假设Redis中键是"store:<store_id>"，值是店铺特征向量的字节数组
        for (int storeId = 1; storeId <= 10; storeId++) {
            byte[] featureBytes = redisTemplate.opsForValue().get("store:" + storeId);
            if (featureBytes != null) {
                INDArray featureVector = deserializeFeatureVector(featureBytes);
                storeVectors.put(storeId, featureVector);
            }
        }
        return storeVectors;
    }

    private INDArray deserializeFeatureVector(byte[] bytes) {
        FloatBuffer floatBuffer = ByteBuffer.wrap(bytes).asFloatBuffer();
        float[] floats = new float[floatBuffer.remaining()];
        floatBuffer.get(floats);
        return Nd4j.create(floats);
    }

    private List<Integer> recommendStores(INDArray userVector, Map<Integer, INDArray> storeVectors) {
        List<Map.Entry<Integer, Double>> similarityScores = storeVectors.entrySet().stream()
            .map(entry -> {
                double similarity = cosineSimilarity(userVector, entry.getValue());
                return new AbstractMap.SimpleEntry<>(entry.getKey(), similarity);
            })
            .sorted(Map.Entry.<Integer, Double>comparingByValue().reversed())
            .collect(Collectors.toList());

        return similarityScores.stream()
            .limit(10)
            .map(Map.Entry::getKey)
            .collect(Collectors.toList());
    }

    private double cosineSimilarity(INDArray vec1, INDArray vec2) {
        double dotProduct = Nd4j.getBlasWrapper().dot(vec1, vec2);
        double norm1 = vec1.norm2Number().doubleValue();
        double norm2 = vec2.norm2Number().doubleValue();
        return dotProduct / (norm1 * norm2);
    }
}

// 存储特征向量到mysql
import java.nio.ByteBuffer;

public class VectorUtils {

    // 将 float[] 转为 byte[]
    public static byte[] floatArrayToByteArray(float[] floatArray) {
        ByteBuffer byteBuffer = ByteBuffer.allocate(4 * floatArray.length);  // float占4字节
        for (float value : floatArray) {
            byteBuffer.putFloat(value);
        }
        return byteBuffer.array();
    }

    // 将 byte[] 转回 float[]
    public static float[] byteArrayToFloatArray(byte[] byteArray) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(byteArray);
        float[] floatArray = new float[byteBuffer.remaining() / 4];  // 每4字节为一个float
        for (int i = 0; i < floatArray.length; i++) {
            floatArray[i] = byteBuffer.getFloat();
        }
        return floatArray;
    }
}


// 4. 数据处理与Redis存储
@Component
public class StoreFeatureLoader {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Autowired
    private RedisTemplate<String, byte[]> redisTemplate;

    @PostConstruct
    public void loadStoreFeaturesToRedis() {
        List<Map<String, Object>> stores = jdbcTemplate.queryForList("SELECT store_id, feature_vector FROM store_features");
        for (Map<String, Object> store : stores) {
            int storeId = (Integer) store.get("store_id");
            byte[] featureVector = (byte[]) store.get("feature_vector");

            // 存入Redis
            redisTemplate.opsForValue().set("store:" + storeId, featureVector);
        }
    }
}
