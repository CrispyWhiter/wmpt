-- MySQL dump 10.13  Distrib 8.0.41, for Win64 (x86_64)
--
-- Host: localhost    Database: wmpt
-- ------------------------------------------------------
-- Server version	8.0.41

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `ad`
--

DROP TABLE IF EXISTS `ad`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ad` (
  `idad` int NOT NULL AUTO_INCREMENT,
  `adtext` text NOT NULL,
  PRIMARY KEY (`idad`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ad`
--

LOCK TABLES `ad` WRITE;
/*!40000 ALTER TABLE `ad` DISABLE KEYS */;
INSERT INTO `ad` VALUES (4,'建平骨灰店开业了！！！'),(5,'建平码没了！！！');
/*!40000 ALTER TABLE `ad` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `menu`
--

DROP TABLE IF EXISTS `menu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `menu` (
  `idmenu` int NOT NULL AUTO_INCREMENT,
  `idrest` int NOT NULL,
  `dish_name` char(25) NOT NULL,
  `dish_price` int NOT NULL,
  PRIMARY KEY (`idmenu`,`idrest`),
  KEY `menu_ibfk_1` (`idrest`),
  CONSTRAINT `menu_ibfk_1` FOREIGN KEY (`idrest`) REFERENCES `rests` (`idrest`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `menu`
--

LOCK TABLES `menu` WRITE;
/*!40000 ALTER TABLE `menu` DISABLE KEYS */;
INSERT INTO `menu` VALUES (3,1,'牛肉',25),(4,1,'鸡肉',12),(5,1,'羊肉',18),(6,1,'11',1),(7,1,'11',1),(8,2,'烤鸡',25),(9,2,'牛肉',15),(10,2,'水煮鱼',12);
/*!40000 ALTER TABLE `menu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order`
--

DROP TABLE IF EXISTS `order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order` (
  `idorder` int NOT NULL AUTO_INCREMENT,
  `iduser` int NOT NULL,
  `idrest` int NOT NULL,
  `status` int NOT NULL,
  PRIMARY KEY (`idorder`),
  KEY `customerid_idx` (`iduser`),
  KEY `restid_idx` (`idrest`),
  CONSTRAINT `customerid` FOREIGN KEY (`iduser`) REFERENCES `users` (`iduser`),
  CONSTRAINT `restid` FOREIGN KEY (`idrest`) REFERENCES `rests` (`idrest`)
) ENGINE=InnoDB AUTO_INCREMENT=71 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order`
--

LOCK TABLES `order` WRITE;
/*!40000 ALTER TABLE `order` DISABLE KEYS */;
INSERT INTO `order` VALUES (14,7,2,3),(24,7,2,3),(25,7,2,2),(26,7,2,2),(27,7,2,2),(28,7,2,3),(29,7,2,3),(33,7,2,3),(34,7,2,3),(35,2,2,2),(36,7,2,3),(46,7,2,3),(47,7,2,2),(48,7,2,2),(49,7,2,2),(50,7,2,3),(51,7,2,2),(52,7,2,3),(54,7,2,2),(55,7,2,2),(58,8,2,2),(59,7,2,2),(61,7,2,2),(62,7,2,3),(63,7,2,2),(64,7,2,2),(65,7,2,2),(66,7,2,2),(67,7,2,2),(68,7,2,2),(70,7,2,-1);
/*!40000 ALTER TABLE `order` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order_commit`
--

DROP TABLE IF EXISTS `order_commit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order_commit` (
  `idorder_commit` int NOT NULL AUTO_INCREMENT,
  `idorder` int NOT NULL,
  `iduser` int NOT NULL,
  `commit` char(200) NOT NULL,
  PRIMARY KEY (`idorder_commit`),
  KEY `idorder_1_idx` (`idorder`),
  KEY `iduser_2_idx` (`iduser`),
  CONSTRAINT `idorder_1` FOREIGN KEY (`idorder`) REFERENCES `order` (`idorder`),
  CONSTRAINT `iduser_2` FOREIGN KEY (`iduser`) REFERENCES `users` (`iduser`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_commit`
--

LOCK TABLES `order_commit` WRITE;
/*!40000 ALTER TABLE `order_commit` DISABLE KEYS */;
INSERT INTO `order_commit` VALUES (13,29,7,'222'),(14,33,7,'333'),(23,46,7,'AAA'),(24,62,7,'AAA'),(25,36,7,'AAA'),(26,14,7,'AAA'),(27,24,7,'AAA'),(28,28,7,'AAA'),(29,52,7,'AAA'),(30,50,7,'AAA');
/*!40000 ALTER TABLE `order_commit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order_data`
--

DROP TABLE IF EXISTS `order_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order_data` (
  `idorder_data` int NOT NULL AUTO_INCREMENT,
  `idorder` int NOT NULL,
  `iddish` int NOT NULL,
  `iduser` int NOT NULL,
  `dish_number` int NOT NULL,
  PRIMARY KEY (`idorder_data`),
  KEY `orderid_idx` (`idorder`),
  KEY `dishid_idx` (`iddish`),
  KEY `userid_idx` (`iduser`),
  CONSTRAINT `dishid` FOREIGN KEY (`iddish`) REFERENCES `menu` (`idmenu`),
  CONSTRAINT `orderid` FOREIGN KEY (`idorder`) REFERENCES `order` (`idorder`),
  CONSTRAINT `userid_1` FOREIGN KEY (`iduser`) REFERENCES `users` (`iduser`)
) ENGINE=InnoDB AUTO_INCREMENT=48 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_data`
--

LOCK TABLES `order_data` WRITE;
/*!40000 ALTER TABLE `order_data` DISABLE KEYS */;
INSERT INTO `order_data` VALUES (1,14,8,7,1),(4,24,8,7,1),(5,25,8,7,1),(6,26,9,7,1),(7,26,8,7,1),(8,27,8,7,1),(9,28,8,7,3),(10,29,8,7,5),(11,29,9,7,1),(13,33,8,7,4),(14,33,9,7,5),(15,34,8,7,1),(16,34,9,7,1),(17,35,8,2,2),(18,35,9,2,5),(19,36,8,7,1),(20,36,9,7,1),(21,46,8,7,1),(22,47,8,7,1),(23,48,8,7,1),(24,49,9,7,1),(25,50,8,7,1),(26,50,9,7,1),(27,51,8,7,1),(28,52,8,7,1),(29,54,9,7,1),(30,55,9,7,1),(31,59,10,7,1),(33,61,8,7,1),(34,61,9,7,1),(35,61,10,7,1),(36,62,8,7,1),(37,62,9,7,2),(38,62,10,7,3),(39,63,8,7,1),(40,64,10,7,1),(41,65,9,7,1),(42,66,8,7,1),(43,67,8,7,1),(44,68,8,7,2),(45,68,9,7,2),(46,68,10,7,5),(47,70,8,7,1);
/*!40000 ALTER TABLE `order_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rests`
--

DROP TABLE IF EXISTS `rests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `rests` (
  `idrest` int NOT NULL AUTO_INCREMENT,
  `restname` char(25) DEFAULT NULL,
  `iduser` int NOT NULL,
  `time_start` time DEFAULT NULL,
  `time_close` time DEFAULT NULL,
  PRIMARY KEY (`idrest`,`iduser`),
  KEY `username_idx` (`iduser`),
  KEY `userid_idx` (`iduser`),
  CONSTRAINT `userid` FOREIGN KEY (`iduser`) REFERENCES `users` (`iduser`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rests`
--

LOCK TABLES `rests` WRITE;
/*!40000 ALTER TABLE `rests` DISABLE KEYS */;
INSERT INTO `rests` VALUES (1,'测试餐厅',5,NULL,NULL),(2,'三饭堂',6,'07:00:00','21:00:00');
/*!40000 ALTER TABLE `rests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `iduser` int NOT NULL AUTO_INCREMENT,
  `name` char(25) NOT NULL,
  `password` char(25) NOT NULL,
  `type` char(10) NOT NULL,
  `is_ban` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`iduser`,`name`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'textrest','123456','1',0),(2,'testcust','1','0',0),(5,'A','1','1',0),(6,'B','1','1',0),(7,'C','1','0',0),(8,'root','1','2',0);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-02-15  1:28:20
