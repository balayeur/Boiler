-- phpMyAdmin SQL Dump
-- version 5.0.4deb2+deb11u1
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost:3306
-- Généré le : mar. 26 mars 2024 à 15:26
-- Version du serveur :  10.5.23-MariaDB-0+deb11u1
-- Version de PHP : 7.4.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `esp_data`
--

-- --------------------------------------------------------

--
-- Structure de la table `BoilerBurnerData`
--

CREATE TABLE `BoilerBurnerData` (
  `id` int(6) UNSIGNED NOT NULL,
  `burner` tinyint(1) NOT NULL,
  `reading_time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `BoilerBurnerData`
--

INSERT INTO `BoilerBurnerData` (`id`, `burner`, `reading_time`) VALUES
(5892, 1, '2024-03-25 20:54:16'),
(5893, 1, '2024-03-25 20:54:34'),
(5894, 1, '2024-03-25 20:55:41'),
(5895, 1, '2024-03-25 20:57:47'),
(5896, 1, '2024-03-25 21:00:02'),
(5897, 1, '2024-03-25 21:03:31'),
(5898, 0, '2024-03-25 21:03:46'),
(5899, 1, '2024-03-25 21:03:59'),
(5900, 1, '2024-03-25 21:11:47'),
(5901, 0, '2024-03-25 21:12:06'),
(5902, 1, '2024-03-25 21:12:17'),
(5903, 1, '2024-03-25 21:12:52'),
(5904, 1, '2024-03-25 21:22:05'),
(5905, 1, '2024-03-25 21:33:08'),
(5906, 1, '2024-03-25 21:41:37'),
(5907, 0, '2024-03-25 21:41:38'),
(5908, 1, '2024-03-25 22:38:09'),
(5909, 0, '2024-03-25 22:38:10'),
(5910, 1, '2024-03-25 22:43:10'),
(5911, 0, '2024-03-25 22:48:08'),
(5912, 1, '2024-03-26 02:21:53'),
(5913, 0, '2024-03-26 02:27:22'),
(5914, 1, '2024-03-26 05:00:46'),
(5915, 0, '2024-03-26 05:05:53'),
(5916, 1, '2024-03-26 05:54:00'),
(5917, 0, '2024-03-26 05:59:42'),
(5918, 1, '2024-03-26 06:40:11'),
(5919, 0, '2024-03-26 06:45:37'),
(5920, 1, '2024-03-26 07:26:48'),
(5921, 0, '2024-03-26 07:32:08'),
(5922, 1, '2024-03-26 08:11:37'),
(5923, 0, '2024-03-26 08:17:06'),
(5924, 1, '2024-03-26 10:59:33'),
(5925, 0, '2024-03-26 11:04:55'),
(5926, 1, '2024-03-26 11:17:23'),
(5927, 0, '2024-03-26 11:17:24'),
(5928, 1, '2024-03-26 12:28:58'),
(5929, 0, '2024-03-26 12:34:21'),
(5930, 1, '2024-03-26 12:55:58'),
(5931, 0, '2024-03-26 12:55:59'),
(5932, 1, '2024-03-26 13:23:30'),
(5933, 0, '2024-03-26 13:28:42');

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `BoilerBurnerData`
--
ALTER TABLE `BoilerBurnerData`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `BoilerBurnerData`
--
ALTER TABLE `BoilerBurnerData`
  MODIFY `id` int(6) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5934;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
