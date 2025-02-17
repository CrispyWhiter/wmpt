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
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-02-15  1:17:32
